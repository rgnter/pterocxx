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


/**
*  asio::io_context asio_ctx;

    asio::ip::tcp::resolver::query remote_query("panel.battleland.eu", "https");
    asio::ip::tcp::resolver resolver(asio_ctx);
    boost::system::error_code ec;
    const auto results = resolver.resolve(remote_query, ec);
    if (ec) {
        printf("Couldn't resolve address '%s': (0x%X) %s\n",
               remote_query.host_name().c_str(),
               ec.value(),
               ec.message().c_str()
        );
        return 1;
    }

    asio::ip::tcp::endpoint remote_endpoint;
    for (const auto &resolved_endpoint: results) {
        if (resolved_endpoint.endpoint().protocol() == asio::ip::tcp::v4())
            remote_endpoint = resolved_endpoint;
    }

    printf("Resolved address '%s':%d - '%s'\n",
           remote_query.host_name().c_str(),
           remote_endpoint.port(),
           remote_endpoint.address().to_string().c_str()
    );

    std::array<uint8_t, 4096> bfr{};
    const auto rsp_buffer = asio::buffer(bfr);
    const auto req_buffer = asio::buffer(
            "GET /api/application/users HTTP/1.1\r\nHost: panel.battleland.eu\r\nAccept: application/json\r\nAuthorization: Bearer mNAnqdY09AdoWCOqlkn9z1Ha5cRJ6ROPLwQBQCfHpTLWPekN\r\n\r\n"
    );

    try {
        ssl::context ssl_ctx(ssl::context::method::tlsv12_client);
        ssl::stream<ip::tcp::socket> connection(asio_ctx, ssl_ctx);


        connection.lowest_layer().connect(remote_endpoint);
        connection.handshake(ssl::stream_base::handshake_type::client);
        connection.write_some(req_buffer);
        connection.read_some(rsp_buffer);

        printf("%s\n", (const char *) bfr.data());
    }catch(const std::exception& x) {
        printf("%s\n", x.what());
    }

    asio_ctx.run();
    return 0;
*/