#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "pterocxx.hpp"


int main() {
   pterocxx::application app("panel.battleland.eu", 443, "mNAnqdY09AdoWCOqlkn9z1Ha5cRJ6ROPLwQBQCfHpTLWPekN");
   app.init();
   app.query_users([](const pterocxx::query_users_response_s& response){
       for (const auto &user : response.users) {
           printf("User: %s\n", user.username.data());
       }
   });
   app.sync();
}
