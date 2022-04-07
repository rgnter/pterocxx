//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_REST_HPP
#define PTEROCXX_REST_HPP

#include <cstdarg>
#include <exception>

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio/ssl.hpp>

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

namespace pterocxx {

    /**
     * REST exception.
     */
    class rest_exception : public std::exception {
    public:
        const std::string message;
    public:
        explicit rest_exception(const std::string &message);
    public:
        const char *what() const noexcept override;
    };

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
     * Structure holding rest_client request
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
     * Structure holding rest_client response
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

    /**
     * Build GET REST request.
     *
     * @param endpoint Request endpoint.
     * @param query    Request query.
     * @param headers  Request headers.
     * @return GET REST request.
     */
    rest_request_s make_get_request(const std::string& endpoint,
                                    const pterocxx::query_s& query,
                                    const std::unordered_map<std::string, std::string>& headers);

    /**
     * Build POST REST request.
     *
     * @param endpoint Request endpoint.
     * @param query    Request query.
     * @param body     Request body.
     * @param headers  Request headers.
     * @return POST REST request.
     */
    rest_request_s make_post_request(const std::string& endpoint,
                                     const pterocxx::query_s& query,
                                     const std::string& body,
                                     const std::unordered_map<std::string, std::string>& headers);

    /**
     * Build UPDATE REST request.
     *
     * @param endpoint Request endpoint.
     * @param query    Request query.
     * @param body     Request body.
     * @param headers  Request headers.
     * @return POST REST request.
     */
    rest_request_s make_patch_request(const std::string& endpoint,
                                      const pterocxx::query_s& query,
                                      const std::string& body,
                                      const std::unordered_map<std::string, std::string>& headers);

    /**
     * Build DELETE REST request.
     *
     * @param endpoint Request endpoint.
     * @param query    Request query.
     * @param body     Request body.
     * @param headers  Request headers.
     * @return POST REST request.
     */
    rest_request_s make_delete_request(const std::string& endpoint,
                                      const pterocxx::query_s& query,
                                      const std::string& body,
                                      const std::unordered_map<std::string, std::string>& headers);

    /**
     * Rest response handler
     */
    typedef std::function<void(rest_response_s)> rest_response_handler_t;

    /**
     * Class providing http client rest_client functionality.
     */
    class rest_client {
    public:
        /**
         * Remote host.
         */
        std::string host;
        /**
         * Remote port.
         */
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
         * @throws pterocxx::rest_exception when hostname is invalid.
         */
        explicit rest_client(const std::string &host, uint16_t port);
        ~rest_client();

    public:
        /**
         * @return Boolean true if client is busy processing requests.
         */
        bool is_busy();
        /**
         * Connects client to the remote.
         * @throws pterocxx::rest_exception when can't connect to remote.
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
