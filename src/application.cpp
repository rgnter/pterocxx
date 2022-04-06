//
// Created by maros on 05/04/2022.
//

#include <pterocxx.hpp>
#include <nlohmann/json.hpp>

namespace pterocxx {

    application::application(std::string host,
                             std::string app_key,
                             uint16_t port,
                             std::string app_name) :
            remote_port(port),
            remote_host(std::move(host)),
            app_key(std::move(app_key)),
            app_name(std::move(app_name)) {
        rest = new pterocxx::rest(this->remote_host, this->remote_port);

        basic_request_headers["Host"] = this->remote_host;
        basic_request_headers["Accept"] = "application/json";
        basic_request_headers["User-Agent"] = this->app_name;
        basic_request_headers["Authorization"] = "Bearer " + this->app_key;
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
        this->should_run.notify_all();
    }

    void application::sync() {
        std::mutex thread_lock;
        std::unique_lock thread_guard(thread_lock);
        should_run.wait(thread_guard);
    }

    void application::get_users(const get_users_response_handler_t &handler) {
        auto request = pterocxx::rest_request_s{
                .method = "GET",
                .endpoint = "/api/application/users",
                .headers = basic_request_headers
        };
        this->rest->request(request, [handler](const rest_response_s &response) {
            const auto& response_json = nlohmann::json::parse(response.body);

            get_users_response_s api_response;
            for (const auto &entry : response_json["data"]) {
                user_s entry_user;
                entry_user.build_from_attributes(entry["attributes"]);
                api_response.users.push_back(entry_user);
            }

            handler(api_response);
        });
    }

    void application::get_user_details(pterocxx::user_id_t user_id,
                                       const get_user_details_response_handler_t &handler,
                                       const pterocxx::query_s& query) {
        auto request = pterocxx::rest_request_s{
                .method = "GET",
                .endpoint = "/api/application/user/",
                .headers = basic_request_headers,
                .query = query
        };

        this->rest->request(request, [handler](const rest_response_s &response) {
            const auto& response_json = nlohmann::json::parse(response.body);
        });
    }


}