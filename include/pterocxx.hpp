//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_PTEROCXX_HPP
#define PTEROCXX_PTEROCXX_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "types.hpp"
#include "rest.hpp"

namespace pterocxx {

    /**
     * Base response
     */
    struct base_response_s {
    public:
        std::vector<pterocxx::error_s> errors;

    public:
        virtual void parse(const nlohmann::json& json);
    public:
        bool has_errors();
    };

    /**
     * Structure holding users response.
     */
    struct get_users_response_s
            : base_response_s {
    public:
        pterocxx::list_object_s<pterocxx::user_s> users;
    public:
        void parse(const nlohmann::json& json);
    };
    /**
     * Get users handler.
     */
    typedef std::function<void(get_users_response_s)> get_users_response_handler_t;


    /**
     * Structure holding user details response.
     */
    struct get_user_details_response_s
            : base_response_s {
    public:
        pterocxx::user_s user;
    public:
        void parse(const nlohmann::json& json);
    };
    /**
     * Get user details handler.
     */
    typedef std::function<void(get_user_details_response_s)> get_user_details_response_handler_t;


    /**
     * Pterodactyl application.
     */
    class application {
    private:
        std::condition_variable should_run;

    private:
        const uint16_t    remote_port;
        const std::string remote_host;
        const std::string app_key;
        const std::string app_name;

    private:
        pterocxx::rest_client* rest;

    public:
        /**
         * Basic request headers used in every request.
         */
        std::unordered_map<std::string, std::string> basic_request_headers;

    public:
        /**
         * Construct application and rest client.
         *
         * @param host     Remote host, for example panel.example.net
         * @param app_key  Application authorization key
         * @param port     Remote port, usually HTTPS port {@code 443}
         * @param app_name Application name, referenced in User-Agent header.
         */
        application(
                std::string host,
                std::string app_key,
                uint16_t port = 0,
                std::string app_name = "pterocxx"
        );

        /**
         * Destruct application and rest_client client.
         */
        ~application();

        /**
         * Initialize application.
         * Connect to remote.
         */
        void init();

        /**
         * Terminate application.
         * Disconnect from remote.
         */
        void term();

        /**
         * Synchronize with application thread.
         * Interrupted when application is terminated.
         */
        void sync();

        /**
         * Get users.
         * API endpoint: <code>/api/application/users</code>
         * @param handler Handler.
         */
        void get_users(const get_users_response_handler_t& handler);

        /**
         * Get user details.
         * API endpoint: <code>/api/application/users/{user_id}</code>
         * @param user_id User ID.
         * @param handler Handler.
         * @param include_servers Include servers in user details.
         */
        void get_user_details(pterocxx::user_id_t user_id,
                              const get_user_details_response_handler_t& handler,
                              bool include_servers = false);

    };

}

#endif //PTEROCXX_PTEROCXX_HPP
