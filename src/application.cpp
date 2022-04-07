//
// Created by maros on 05/04/2022.
//

#include <fmt/format.h>
#include <pterocxx.hpp>
#include <nlohmann/json.hpp>


namespace pterocxx {


    void base_response_s::parse(const nlohmann::json &json) {
        if (json.contains("errors")) {
            for (const auto &error_json: json["errors"]) {
                pterocxx::error_s error;
                error.build_from_attributes(error_json);
                errors.emplace_back(error);
            }
        }
    }

    void get_users_response_s::parse(const nlohmann::json &json) {
        base_response_s::parse(json);
        pterocxx::list_object_s<pterocxx::user_s> user_list;
        user_list.build_from_attributes(json);
        this->users = std::move(user_list);
    }

    void get_user_details_response_s::parse(const nlohmann::json &json) {
        base_response_s::parse(json);
        if (json.contains("attributes")) {
            user_s user;
            user.build_from_attributes(json["attributes"]);
            this->user = user;
        }
    }

    bool base_response_s::has_errors() {
        return !this->errors.empty();
    }

    application::application(std::string host,
                             std::string app_key,
                             uint16_t port,
                             std::string app_name) :
            remote_port(port),
            remote_host(std::move(host)),
            app_key(std::move(app_key)),
            app_name(std::move(app_name)) {

        // rest_client client
        rest = new pterocxx::rest_client(this->remote_host, this->remote_port);

        // basic headers
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
        this->rest->request(request, [request, handler](const rest_response_s &response) {
            try {
                const auto response_json = nlohmann::json::parse(response.body);
                pterocxx::get_users_response_s api_response;
                api_response.parse(response_json);
                handler(api_response);
            } catch (const std::exception &x) {
                printf("API endpoint %s ERROR: %s\n\tResponse Body: %s\n", request.endpoint.c_str(), x.what(),
                       response.body.c_str());
            }

        });
    }

    void application::get_user_details(pterocxx::user_id_t user_id,
                                       const get_user_details_response_handler_t &handler,
                                       bool include_servers) {
        auto query = pterocxx::query_s();
        if (include_servers)
            query["include"] += "servers";

        auto request = make_get_request(fmt::format("/api/application/users/{}", user_id),
                                        query,
                                        basic_request_headers
        );

        this->rest->request(request, [request, handler](const rest_response_s &response) {
            try {
                const auto &response_json = nlohmann::json::parse(response.body);

                {
                    pterocxx::get_user_details_response_s api_response;
                    handler(api_response);
                }
            } catch (const std::exception &x) {
                printf("API endpoint %s ERROR: %s\n\tResponse Body: %s\n", request.endpoint.c_str(), x.what(),
                       response.body.c_str());
            }
        });
    }

}