//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_REST_HPP
#define PTEROCXX_REST_HPP


#include <cstdarg>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio/ssl.hpp>



namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;


namespace pterocxx {

    /**
     * Structure holding buildable query parameters.
     */
    struct query_s {
    private:
        std::unordered_map<std::string, std::string> query;

    public:
        /**
         * @return True if query has any parameters. Otherwise, returns false.
         */
        [[nodiscard]] bool present() const;
        /**
         * Builds query string.
         * @retur Query string, ie. Key0=Val0&Key1=Val1
         */
        [[nodiscard]] std::string build() const;

        /**
         * Subscript operator.
         * @param key Key.
         * @return Value.
         */
        std::string& operator[](const std::string& key);
    };

    /**
     * Structure holding rest request
     */
    struct rest_request_s {
    public:
        std::string body;
        std::string method;
        std::string endpoint;
    public:
        std::unordered_map<std::string, std::string> headers;
    };

    /**
     * Structure holding rest response
     */
    struct rest_response_s {
    public:
        std::string version;
        std::string body;
        std::string detail;

        uint32_t status_code = 0;
    public:

        std::unordered_map<std::string, std::string> headers;
    };


    rest_request_s make_get_request(const std::string& endpoint,
                                    const pterocxx::query_s& query,
                                    const std::unordered_map<std::string, std::string>& headers);

    rest_request_s make_post_request(const std::string& endpoint,
                                     const std::string& body,
                                     const std::unordered_map<std::string, std::string>& headers);
    /**
     * Rest response handler
     */
    typedef std::function<void(rest_response_s)> rest_response_handler_t;

    /**
     * Class providing http client rest functionality.
     */
    class rest {
    private:
        std::string host;
        uint16_t port;

    private:
        ssl::context ssl_ctx{
                ssl::context::method::tlsv12_client
        };
        asio::io_context io_ctx{};

    private:
        std::mutex io_lock;
        asio::ip::tcp::endpoint remote;
        ssl::stream<asio::ip::tcp::socket> *connection
                = nullptr;

    public:
        /**
         * Constructs http client.
         *
         * @param host Host to lookup.
         * @param port Port.
         */
        explicit rest(const std::string &host, uint16_t port);
        ~rest();

    public:
        /**
         * Connects client to the remote.
         */
        void init();
        /**
         * Disconnects client from the remote.
         */
        void term();

        /**
         * Sends REST request to the remote.
         * @param request  Request.
         * @param response_handler Response Handler.
         */
        void request(const rest_request_s &request,
                     const rest_response_handler_t &response_handler);
    };

}

#endif //PTEROCXX_REST_HPP
