//
// Created by maros on 05/04/2022.
//

#include <pterocxx.hpp>


namespace pterocxx {

    application::application(std::string address,
                             uint16_t port,
                             std::string app_key,
                             std::string app_name) :
            remote_port(port),
            remote_host(std::move(address)),
            app_key(std::move(app_key)),
            app_name(std::move(app_name)) {
        rest = new pterocxx::rest(this->remote_host, this->remote_port);
    }

    application::~application() {
        delete rest;
    }

    void application::init() {
        if (this->rest == nullptr)
            return;
        this->rest->init();
    }

    void application::term() {

    }

    void application::sync() {
        std::mutex thread_lock;
        std::unique_lock thread_guard(thread_lock);
        running.wait(thread_guard);
    }

    void application::query_users(const query_users_response_handler_t &handler) {
        std::unordered_map<std::string, std::string> headers;
        headers["Host"] = this->remote_host;
        headers["Accept"] = "application/json";
        headers["Authorization"] = "Bearer " + this->app_key;

        auto request = pterocxx::rest_request_s{
                .method = "GET",
                .endpoint = "/api/application/users",
                .headers = headers
        };
        this->rest->request(request, [](const rest_response_s &response) {
            response.body; // parse json
        });
    }


}