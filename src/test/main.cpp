
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
    pterocxx::application app("panel.example.net",
                              "app_key"
            // port = 443,          Server port
            // app_name="pterocxx"  Used in user agent when sending requests
    );

    app.init();
    app.cr([](const pterocxx::delete_user_response_s& response) {
        if(!response.successful)
            for (const auto &error : response.errors) {
                printf("Error: %s, %s, %s\n", error.detail.c_str(), error.code.c_str(), error.status.c_str());
            }
    });

    app.sync();
}
