//
// Created by maros on 06/04/2022.
//

#include "types.hpp"


namespace pterocxx {

    void user_s::build_from_attributes(const nlohmann::json& attributes) {
        this->id = attributes["id"];
        this->external_id = attributes["external_id"];
        this->uuid = attributes["uuid"];
        this->username = attributes["username"];
        this->email = attributes["email"];
        this->first_name = attributes["first_name"];
        this->last_name = attributes["last_name"];
        this->language = attributes["language"];
        this->root_admin = attributes["root_admin"];
        this->two_factor_auth = attributes["2fa"];
        this->created_at = attributes["created_at"];
        this->updated_at = attributes["updated_at"];
    }
}