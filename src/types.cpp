//
// Created by maros on 06/04/2022.
//

#include "types.hpp"


namespace pterocxx {

    /**
     *
     * @tparam T
     * @param key
     * @param default_value
     * @param attributes
     * @return
     */
    template<typename T>
    nlohmann::json::value_type
    read_attribute(const T &key, nlohmann::json::value_type default_value, const nlohmann::json &attributes) {
        auto value = attributes[key];
        if (value.is_null())
            return default_value;
        return value;
    }

    void user_s::build_from_attributes(const nlohmann::json &attributes) {
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
        this->root_admin      = attributes["root_admin"];
        this->two_factor_auth = attributes["2fa"];
        this->created_at = std::string(
                read_attribute("created_at", "null", attributes)
        );
        this->updated_at = std::string(
                read_attribute("updated_at", "null", attributes)
        );
    }
}