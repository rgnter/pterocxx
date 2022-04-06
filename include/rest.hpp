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
     * Query entry.
     */
    typedef std::pair<std::string, nlohmann::json::value_type> query_entry_t;

    /**
     * Structure holding buildable query parameters.
     */
    struct query_s {
    private:
        std::unordered_map<std::string, nlohmann::json::value_type> query;

    public:
        [[nodiscard]] inline bool present() const {
            return !query.empty();
        }
        [[nodiscard]] inline std::string build() const {
            std::string built;
            for (const auto &item : query) {
                built+=item.first;
                built+="=";
                built+=item.second.dump();
            }
            return built;
        }
        inline nlohmann::json::value_type& operator[](const std::string& key) {
            return query[key];
        }

    };

    static query_s make_query(query_entry_t entries...) {
        va_list args;
        va_start(args, entries);
        while(true) {
            query_entry_t& entry = va_arg(args, query_entry_t);
            printf("%s\n", entry.first.c_str());
        }
        va_end(args);
    }

    /**
     * Structure holding rest request
     */
    struct rest_request_s {
    public:
        std::string method;
        std::string endpoint;

        std::unordered_map<std::string, std::string> headers;
        std::string body;

        pterocxx::query_s query;
    };

    /**
     * Structure holding rest response
     */
    struct rest_response_s {
    public:
        std::string version;
        uint32_t status_code = 0;
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
        /**
         * Constructs REST connection.
         *
         * @param host Host to lookup
         * @param port Port
         */
        explicit rest(const std::string &host, uint16_t port);
        virtual ~rest();

    public:
        /**
         * Initializes REST connection to server.
         * @return After connection is ready to accept requests.
         */
        void init();
        /**
         * Terminates REST connection to server.
         * @return Immediately
         */
        void term();

        /**
         * Sends REST request
         * @param request  Request
         * @param response_handler Response Handler
         */
        void request(const rest_request_s &request,
                     const rest_response_handler_t &response_handler);
    };

}

#endif //PTEROCXX_REST_HPP
