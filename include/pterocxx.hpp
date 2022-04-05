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
     * Structure holding query users response.
     */
    struct query_users_response_s {
    public:
        std::vector<pterocxx::user_s> users;
    };

    /**
     * Query users handler.
     */
    typedef std::function<void(query_users_response_s)> query_users_response_handler_t;

    /**
     * Application connecting to Pterodactyl API.
     */
    class application {
    private:
        std::condition_variable running;

    private:
        const uint16_t    remote_port;
        const std::string remote_host;
        const std::string app_key;
        const std::string app_name;

    private:
        pterocxx::rest* rest;

    public:
        /**
         * Constructs application.
         *
         * @param address Remote host, ie. {@code panel.example.com}
         * @param port    Remote port, usually HTTPS port {@code 443}
         * @param app_key Application authorization key
         * @param app_name Application name, referenced in User-Agent header.
         */
        application(
                std::string address,
                uint16_t port,
                std::string app_key,
                std::string app_name = "pterocxx"
        );
        ~application();

        void init();
        void term();
        void sync();

        void query_users(const query_users_response_handler_t& handler);

    };

}

#endif //PTEROCXX_PTEROCXX_HPP
