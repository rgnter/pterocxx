
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
    // constructs application on panel.example.net
    // uses default https port so the rest client is using secured connection
    pterocxx::application app("panel.battleland.eu",
                            provide_app_key()
            // port = 443,          Server port
            // app_name="pterocxx"  Used in user agent when sending requests
    );

    app.init();
    app.get_users([](const pterocxx::get_users_response_s& response) {
        for (const auto &item : response.users.data) {
            pterocxx::user_cache.store(item.id, item);
        }
        printf("%s\n", pterocxx::user_cache.retrieve(18)->username.c_str());
    });

    app.sync();
}
