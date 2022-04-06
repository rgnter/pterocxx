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



    // get user
    app.get_user_details(3, [](const pterocxx::get_user_details_response_s &response) {
        printf("%d\n", response.user.id);
    }, true);

    app.sync();
}
