//
// Created by maros on 06/04/2022.
//

#include "types.hpp"


namespace pterocxx {

    template<typename T>
    nlohmann::json::value_type
    read_attribute(const T &key, nlohmann::json::value_type default_value, const nlohmann::json &attributes) {
        auto value = attributes[key];
        if (value.is_null())
            return default_value;
        return value;
    }



    void error_s::read_object(const nlohmann::json &attributes) {
        this->code = std::string(attributes["code"]);
        this->detail = std::string(attributes["detail"]);
        this->status = std::string(attributes["status"]);
    }

    nlohmann::json error_s::write_object() const {
        return nlohmann::json();
    }

    void limits_s::read_object(const nlohmann::json &attributes) {
        this->cpu = attributes["cpu"];
        this->disk = attributes["disk"];
        this->io = attributes["io"];
        this->memory = attributes["memory"];
        this->oom_disabled = attributes["oom_disabled"];
        this->swap = attributes["swap"];
        //this->threads = attributes["threads"]; -- idk whats the type
    }

    nlohmann::json limits_s::write_object() const {
        return nlohmann::json();
    }

    void feature_limits_s::read_object(const nlohmann::json &attributes) {
        this->allocations = attributes["allocations"];
        this->backups = attributes["backups"];
        this->databases = attributes["databases"];
    }

    nlohmann::json feature_limits_s::write_object() const {
        return nlohmann::json();
    }

    void location_s::read_object(const nlohmann::json &attributes) {
        this->id = attributes["id"];
        this->short_description = std::string(read_attribute("short", "null", attributes));
        this->long_description = std::string(read_attribute("long", "null", attributes));
        this->created_at = std::string(read_attribute("created_at", "null", attributes));
        this->updated_at = std::string(read_attribute("updated_at", "null", attributes));
    }

    nlohmann::json location_s::write_object() const {
        return nlohmann::json();
    }

    void node_s::read_object(const nlohmann::json &attributes) {
        this->id = attributes["id"];
        this->uuid = std::string(read_attribute("uuid", "null", attributes));
        this->is_public = attributes["public"];

        this->name = std::string(read_attribute("name", "null", attributes));
        this->description = std::string(read_attribute("description", "null", attributes));

        this->location_id = attributes["location_id"];
        this->fqdn = std::string(read_attribute("fqdn", "null", attributes));
        this->scheme = std::string(read_attribute("scheme", "null", attributes));
        this->is_behind_proxy = attributes["behind_proxy"];
        this->is_maintenance_mode = attributes["maintenance_mode"];

        this->memory = attributes["memory"];
        this->memory_overallocate = attributes["memory_overallocate"];
        this->memory = attributes["cpu"];
        this->memory_overallocate = attributes["cpu_overallocate"];
        this->memory = attributes["disk"];
        this->memory_overallocate = attributes["disk_overallocate"];

        this->upload_size = attributes["upload_size"];
        this->daemon_listen = attributes["daemon_listen"];
        this->daemon_sftp = attributes["daemon_sftp"];
        this->daemon_base = std::string(read_attribute("daemon_base", "null", attributes));

        this->created_at = std::string(read_attribute("created_at", "null", attributes));
        this->updated_at = std::string(read_attribute("updated_at", "null", attributes));
    }

    nlohmann::json server_s::write_object() const {
        return nlohmann::json();
    }

    void allocation_s::read_object(const nlohmann::json &attributes) {

    }

    nlohmann::json allocation_s::write_object() const {
        return nlohmann::json();
    }

    nlohmann::json node_s::write_object() const {
        return nlohmann::json();
    }

    void server_s::read_object(const nlohmann::json &attributes) {

    }

    user_s& user_s::set_username(const std::string &username) {
        this->username = username;
        return *this;
    }

    user_s& user_s::set_email(const std::string &email) {
        this->email = email;
        return *this;
    }

    user_s& user_s::set_first_name(const std::string &first_name) {
        this->first_name = first_name;
        return *this;
    }

    user_s& user_s::set_last_name(const std::string &last_name) {
        this->last_name = last_name;
        return *this;
    }

    user_s& user_s::set_password(const std::string &password) {
        this->password = password;
        return *this;
    }

    void user_s::read_object(const nlohmann::json &attributes) {
        this->id = attributes["id"];
        this->external_id = std::string(
                read_attribute("external_id", "null", attributes)
        );
        this->uuid = std::string(
                read_attribute("uuid", "null", attributes)
        );
        this->username = std::string(
                read_attribute("username", "null", attributes)
        );
        this->email = std::string(
                read_attribute("email", "null", attributes));
        this->first_name = std::string(
                read_attribute("first_name", "null", attributes)
        );
        this->last_name = std::string(
                read_attribute("last_name", "null", attributes)
        );
        this->language = std::string(
                read_attribute("language", "null", attributes)
        );
        this->root_admin = attributes["root_admin"];
        this->two_factor_auth = attributes["2fa"];
        this->created_at = std::string(
                read_attribute("created_at", "null", attributes)
        );
        this->updated_at = std::string(
                read_attribute("updated_at", "null", attributes)
        );
    }

    nlohmann::json user_s::write_object() const {
        nlohmann::json attributes;
        {
            attributes["email"] = this->email;
            attributes["username"] = this->username;
            attributes["first_name"] = this->first_name;
            attributes["last_name"] = this->last_name;
            if(!this->password.empty())
                attributes["password"] = this->password;
        }

        return attributes;
    }


}