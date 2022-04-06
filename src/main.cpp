#include <cstdio>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "pterocxx.hpp"

std::string provide_app_key() {
    std::ifstream file_stream("app_key");
    std::string key;
    std::getline(file_stream, key);
    return key;
}

int main() {
    pterocxx::application app("panel.battleland.eu", provide_app_key());
    app.init();

    // get users
    app.get_users([&app](const pterocxx::get_users_response_s &response) {
        printf("Users1 -- \n");
        for (const auto &user: response.users) {
            printf("User: %s\n", user.username.data());
        }
    });

    // ger user
    app.get_user_details(1, [](const pterocxx::get_user_details_response_s &response) {

    }, pterocxx::make_query({"servers", true}));

    app.sync();
}
