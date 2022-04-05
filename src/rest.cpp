//
// Created by maros on 05/04/2022.
//

#include "rest.hpp"
#include <charconv>

namespace pterocxx {


    rest_response_s parse_raw_response(const uint8_t * data,
                                       size_t data_length) {
        rest_response_s response{};
        return response;
    }

    rest::rest(const std::string& host,
               const uint16_t port) : host(host), port(port) {
        {
            boost::system::error_code error;

            asio::ip::tcp::resolver::query resolver_query(host, "https");
            asio::ip::tcp::resolver resolver(io_ctx);
            auto results = resolver.resolve(resolver_query, error);
            if(error) {
                printf("Rest ctor ERROR: Couldn't resolve address / %d - %s\n", error.value(), error.message().c_str());
                return;
            }

            // could be better ipv6 and ipv4 resolution
            for(const auto& endpoint : results) {
               this->remote = endpoint;
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
        delete connection;
    }

    void rest::init() {
        if(connection == nullptr)
            return;

        boost::system::error_code error;
        this->connection->lowest_layer().connect(this->remote, error);
        if(error) {
            printf("Rest connect ERROR:  %d - %s\n", error.value(), error.message().c_str());
            return;
        }
        this->connection->handshake(ssl::stream_base::handshake_type::client);
        this->io_ctx.run();
    }

    void rest::request(const rest_request_s& request,
                       const rest_response_handler_t& response_handler) {
        if(this->connection == nullptr)
            return;

        asio::post([this, request, response_handler]() {
            auto lock = std::lock_guard<std::mutex>(this->io_lock);
            // write request
            {
                std::string raw_request;
                {
                    // method, endpoint, version
                    raw_request += request.method;
                    raw_request += " ";
                    raw_request += request.endpoint;
                    raw_request += " ";
                    raw_request += "HTTP/1.1";
                    raw_request += "\r\n";
                }
                // a
                for (const auto &header: request.headers) {
                    raw_request += header.first;
                    raw_request += ": ";
                    raw_request += header.second;
                    raw_request += "\r\n";
                }
                // body
                if (!request.body.empty())
                    raw_request += request.body;
                // end
                raw_request += "\r\n";

                this->connection->write_some(asio::const_buffer(raw_request.data(), raw_request.size()));
            }


            rest_response_s response;
            std::string raw_response;

            auto defrag = [&response, &raw_response]() -> bool {

                return false;
            };
            // read response
            {
                std::array<uint8_t, 2048> fragment_buffer{};
                size_t read;
                size_t total = 0;
                do {
                   read = this->connection->read_some(asio::buffer(fragment_buffer));
                   total+=read;

                   // append fragment to raw response
                   raw_response.append(fragment_buffer.data(),
                                       fragment_buffer.data()+read);
                } while(!defrag() /* try to de-fragment the response */);


                printf("total: %lld\n", total);
            }

        });
    }
}