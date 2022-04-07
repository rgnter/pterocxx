
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
    pterocxx::application app("paasdnel.battleland.eu", provide_app_key());
    app.init();

    app.get_users([](const pterocxx::get_users_response_s& response) {
        for (const auto &user : response.users.data) {
            printf("%s\n", user.first_name.c_str());
        }
    });

    app.sync();
}
