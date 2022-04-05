//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_REST_HPP
#define PTEROCXX_REST_HPP


#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio/ssl.hpp>


namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;


namespace pterocxx {

    /**
     * Structure holding rest request
     */
    struct rest_request_s {
    public:
        std::string method;
        std::string endpoint;

        std::unordered_map<std::string, std::string> headers;
        std::string body;
    };

    /**
     * Structure holding rest response
     */
    struct rest_response_s {
    public:
        std::string version;
        uint32_t status_code = -1;
        std::string status_detail;

        std::string body;
        std::unordered_map<std::string, std::string> headers;
    };

    /**
     * Rest response handler
     */
    typedef std::function<void(rest_response_s)> rest_response_handler_t;


    /**
     * Class providing rest functions
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
        explicit rest(const std::string &host, uint16_t port);
        virtual ~rest();

    public:
        void init();
        void request(const rest_request_s &request,
                     const rest_response_handler_t &response_handler);
    };

}

#endif //PTEROCXX_REST_HPP
