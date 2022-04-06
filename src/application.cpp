//
// Created by maros on 05/04/2022.
//

#include <fmt/format.h>
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

        // rest client
        rest = new pterocxx::rest(this->remote_host, this->remote_port);

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

    std::vector<pterocxx::error_s> find_errors(const nlohmann::json &json) {
        std::vector<pterocxx::error_s> errors;
        if (json.contains("errors")) {
            for (const auto &error_json: json["errors"]) {
                errors.emplace_back(pterocxx::error_s{
                        .code = std::string(error_json["code"]),
                        .detail = std::string(error_json["detail"]),
                        .status = std::string(error_json["status"])
                });
            }
        }
        return errors;
    }


    void application::get_users(const get_users_response_handler_t &handler) {
        auto request = pterocxx::rest_request_s{
                .method = "GET",
                .endpoint = "/api/application/users",
                .headers = basic_request_headers
        };
        this->rest->request(request, [request, handler](const rest_response_s &response) {
            try {
                const auto &response_json = nlohmann::json::parse(response.body);

                get_users_response_s api_response;
                api_response.errors = std::move(find_errors(response_json));

                for (const auto &entry: response_json["data"]) {
                    user_s entry_user;
                    entry_user.build_from_attributes(entry["attributes"]);
                    api_response.users.push_back(entry_user);
                }

                handler(api_response);
            } catch (const std::exception &x) {
                printf("API endpoint %s ERROR: %s\n", request.endpoint.c_str(), x.what());
            }

        });
    }

    void application::get_user_details(pterocxx::user_id_t user_id,
                                       const get_user_details_response_handler_t &handler,
                                       bool include_servers) {
        auto query = pterocxx::query_s();
        if(include_servers)
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
                    api_response.errors = std::move(find_errors(response_json));

                    pterocxx::user_s user;
                    if (response_json.contains("attributes") &&
                        response_json.contains("object") &&
                        response_json["object"] == "user") {
                        user.build_from_attributes(response_json["attributes"]);
                        api_response.user = std::move(user);
                    }

                    handler(api_response);
                    printf("%s\n", response_json.dump(4).c_str());
                }
            } catch (const std::exception &x) {
                printf("API endpoint %s ERROR: %s\n", request.endpoint.c_str(), x.what());
            }
        });
    }


}