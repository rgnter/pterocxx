//
// Created by maros on 05/04/2022.
//

#include "rest.hpp"

#include <fmt/format.h>
#include <charconv>
#include <iostream>
#include <regex>

constexpr const char *CLRF = "\r\n";

namespace pterocxx {


    rest_request_s
    make_get_request(const std::string &endpoint,
                     const pterocxx::query_s &query,
                     const std::unordered_map<std::string, std::string> &headers) {
        rest_request_s request;
        {
            request.method = "GET";
            request.headers = headers;
            if(query.present())
                request.endpoint = fmt::format("{}?{}", endpoint, query.build());
            else
                request.endpoint = endpoint;
        }
        return request;
    }

    rest_request_s make_post_request(const std::string &endpoint, const std::string &body,
                                     const std::unordered_map<std::string, std::string> &headers) {
        return rest_request_s();
    }

    /**
     * 
     * @param request 
     * @return 
     */
    std::string gen_raw_request(const pterocxx::rest_request_s &request) {
        std::string raw_request;

        {
            // method, endpoint, version
            raw_request += request.method;
            raw_request += " ";
            raw_request += request.endpoint;
            raw_request += " ";
            raw_request += "HTTP/1.1";
            raw_request += CLRF;
        }
        // headers
        for (const auto &header: request.headers) {
            raw_request += header.first;
            raw_request += ": ";
            raw_request += header.second;
            raw_request += CLRF;
        }
        raw_request += CLRF;
        // body

        if (!request.body.empty())
            raw_request += request.body;

        return raw_request;
    }

    /**
     * 
     * @param raw_response 
     * @return 
     */
    rest_response_s parse_raw_response(const std::string &raw_response) {
        rest_response_s response{};

        // regex matches
        std::regex line_regex(CLRF);
        std::regex status_regex(" ");
        std::regex header_regex(":");

        // line iterator
        std::sregex_token_iterator line_iterator(raw_response.begin(),
                                                 raw_response.end(),
                                                 line_regex, -1);
        std::sregex_token_iterator line_iterator_end;
        uint32_t line_index = 0;
        do {
            line_index++;
            std::string line = *line_iterator++;
            if (line_index == 1) {
                try {
                    // read status header
                    std::sregex_token_iterator entry_iterator(line.begin(),
                                                              line.end(),
                                                              status_regex, -1);
                    std::string version = *entry_iterator;
                    std::string status_code_raw = *++entry_iterator;
                    std::string status_detail = *++entry_iterator;

                    uint32_t status_code = 0;
                    std::from_chars(status_code_raw.c_str(),
                                    status_code_raw.c_str() + status_code_raw.length(),
                                    status_code);
                    response.version = std::move(version);
                    response.status_code = status_code;
                    response.detail = std::move(status_detail);

                } catch (const std::exception &ignored) {
                    throw std::exception("Invalid HTTP status header");
                }
            } else {
                try {
                    std::sregex_token_iterator header_iterator(line.begin(),
                                                               line.end(),
                                                               header_regex, -1);
                    std::sregex_token_iterator header_iterator_end;

                    std::string key = *header_iterator++;
                    if (header_iterator == header_iterator_end)
                        break;
                    std::string val = *header_iterator;
                    if (key.starts_with(" "))
                        key = key.substr(1);
                    if (val.starts_with(" "))
                        val = val.substr(1);
                    if (key.ends_with(" "))
                        key = key.substr(0, key.length() - 2);
                    if (val.ends_with(" "))
                        val = val.substr(0, val.length() - 2);

                    response.headers[key] = val;
                } catch (const std::exception &ignored) {
                    throw std::exception("Invalid HTTP header");
                }
            }

        } while (line_iterator != line_iterator_end);
        return response;
    }

    bool query_s::present() const {
        return !query.empty();
    }

    std::string query_s::build() const {
        std::string built;
        for (const auto &item: query) {
            built += item.first;
            built += "=";
            built += item.second;
            built += "&";
        }
        return built;
    }

    std::string &query_s::operator[](const std::string &key) {
        return query[key];
    }

    rest::rest(const std::string &host,
               const uint16_t port) : host(host), port(port) {
        {
            boost::system::error_code error;

            asio::ip::tcp::resolver::query resolver_query(host, "https");
            asio::ip::tcp::resolver resolver(io_ctx);
            auto results = resolver.resolve(resolver_query, error);
            if (error) {
                printf("Rest ctor ERROR: Couldn't resolve address / %d - %s\n", error.value(), error.message().c_str());
                return;
            }

            // could be better ipv6 and ipv4 resolution
            for (const auto &endpoint: results) {
                this->remote = endpoint;
                if (port)
                    this->remote.port(port);
                break;
            }
        }

        // create ssl socket
        this->connection = new ssl::stream<asio::ip::tcp::socket>(this->io_ctx,
                                                                  this->ssl_ctx);
        SSL_set_tlsext_host_name(connection->native_handle(), host.c_str());
    }

    rest::~rest() {
        // wait for requests to finish
        auto lock = std::lock_guard<std::mutex>(this->io_lock);
        delete connection;
    }

    void rest::init() {
        if (connection == nullptr)
            return;

        boost::system::error_code error;
        this->connection->lowest_layer().connect(this->remote, error);
        if (error) {
            printf("Rest connect ERROR:  %d - %s\n", error.value(), error.message().c_str());
            return;
        }
        this->connection->handshake(ssl::stream_base::handshake_type::client);
        this->io_ctx.run();
    }

    void rest::term() {
        if (connection == nullptr)
            return;
        this->connection->shutdown();
    }

    void rest::request(const rest_request_s &request,
                       const rest_response_handler_t &response_handler) {
        if (this->connection == nullptr)
            return;

        asio::post([this, request, response_handler]() {
            auto lock = std::lock_guard<std::mutex>(this->io_lock);

            // write request
            {
                std::string raw_request = gen_raw_request(request);
                this->connection->write_some(
                        asio::const_buffer(raw_request.data(), raw_request.size())
                );
            }

            // read response
            {
                rest_response_s response;
                std::string raw_response;

                std::array<uint8_t, 1> byte_buffer{};
                size_t total = 0;
                do {
                    this->connection->read_some(asio::buffer(byte_buffer));
                    total++;

                    // append fragment to raw response
                    raw_response.append(byte_buffer.data(),
                                        byte_buffer.data() + 1);
                    if (raw_response.ends_with("\r\n\r\n"))
                        break;
                } while (true);

                try {
                    // parse http headers
                    response = parse_raw_response(raw_response);
                } catch (const std::exception &x) {
                    printf("Rest request ERROR: %s\n", x.what());
                    return;
                }

                // read body
                uint32_t content_length = 0;
                {
                    auto content_length_raw = response.headers["Content-Length"];
                    std::from_chars(content_length_raw.c_str(),
                                    content_length_raw.c_str() + content_length_raw.length(),
                                    content_length);
                }

                std::string body_buffer;
                body_buffer.resize(content_length);
                {
                    asio::read(*this->connection, asio::buffer(body_buffer));
                }

                response.body = std::move(body_buffer);
                response_handler(response);
            }
        });
    }
}