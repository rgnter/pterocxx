
#ifdef WIN32
#define _WIN32_WINNT 0x0601
#endif

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

    pterocxx::user_s user;
    user.set_email("example@example.net")
        .set_username("username")
        .set_last_name("last_name")
        .set_first_name("first_name");
    app.create_user(user, [&app](const auto response) {
        app.delete_user(response.user.id, [](const pterocxx::delete_user_response_s& response) {

        });
    });


    app.sync();
}
