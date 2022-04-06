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
     * Structure holding users response.
     */
    struct get_users_response_s {
    public:
        std::vector<pterocxx::user_s> users;
    };
    /**
     * Query users handler.
     */
    typedef std::function<void(get_users_response_s)> get_users_response_handler_t;


    /**
     * Structure holding get user details response.
     */
    struct get_user_details_response_s {
    public:
        pterocxx::user_s user;
    };
    /**
     * Query users handler.
     */
    typedef std::function<void(get_user_details_response_s)> get_user_details_response_handler_t;

    /**
     * Application connecting to Pterodactyl API.
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
        pterocxx::rest* rest;

    private:
        std::unordered_map<std::string, std::string> basic_request_headers;

    public:
        /**
         * Constructs application.
         *
         * @param host Remote host, ie. {@code panel.example.com}
         * @param port    Remote port, usually HTTPS port {@code 443}
         * @param app_key Application authorization key
         * @param app_name Application name, referenced in User-Agent header.
         */
        application(
                std::string host,
                std::string app_key,
                uint16_t port = 0,
                std::string app_name = "pterocxx"
        );

        /**
         * Destructs application.
         */
        ~application();

        /**
         * Initializes application.
         */
        void init();

        /**
         * Terminates application.
         */
        void term();

        /**
         * Synchronizes with application thread.
         */
        void sync();

        /**
         * Gets users. {@code /api/application/users}
         * @param handler Handler.
         */
        void get_users(const get_users_response_handler_t& handler);

        /**
         * Gets user details. {@code /api/application/users/{user_id}}
         * @param user_id User ID.
         * @param handler Handler.
         * @param query   Optional query.
         */
        void get_user_details(pterocxx::user_id_t user_id,
                              const get_user_details_response_handler_t& handler,
                              const pterocxx::query_s& query = pterocxx::query_s());
    };

}

#endif //PTEROCXX_PTEROCXX_HPP
