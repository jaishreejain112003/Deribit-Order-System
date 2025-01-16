// #include <websocketpp/config/asio_client.hpp>
// #include <websocketpp/client.hpp>
// #include <iostream>

// typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
// typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

// using websocketpp::lib::placeholders::_1;
// using websocketpp::lib::placeholders::_2;
// using websocketpp::lib::bind;

// void on_open(websocketpp::connection_hdl hdl, client* c) {
//     std::cout << "WebSocket connection opened!" << std::endl;
//      websocketpp::lib::error_code ec;
//     client::connection_ptr con = c->get_con_from_hdl(hdl, ec);

//     if (ec) {
//         std::cout << "Failed to get connection pointer: " << ec.message() << std::endl;
//         return;
//     }
//     std::string payload = "{\"userKey\":\"API_KEY\", \"symbol\":\"EURUSD,GBPUSD\"}";
//     c->send(con, payload, websocketpp::frame::opcode::text);
// }

// void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
//     std::cout << "Received message: " << msg->get_payload() << std::endl;
// }

// void on_fail(websocketpp::connection_hdl hdl) {
//     std::cout << "WebSocket connection failed!" << std::endl;
// }

// void on_close(websocketpp::connection_hdl hdl) {
//     std::cout << "WebSocket connection closed!" << std::endl;
// }

// context_ptr on_tls_init(const char * hostname, websocketpp::connection_hdl) {
//     context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
//     try {
//         ctx->set_options(boost::asio::ssl::context::default_workarounds |
//                          boost::asio::ssl::context::no_sslv2 |
//                          boost::asio::ssl::context::no_sslv3 |
//                          boost::asio::ssl::context::single_dh_use);
//     } catch (std::exception& e) {
//         std::cout << "TLS Initialization Error: " << e.what() << std::endl;
//     }

//     return ctx;
// }

// int main(int argc, char* argv[]) {
//     client c;
//     std::string hostname = "marketdata.tradermade.com/feedadv";
//     std::string uri = "wss://" + hostname;

//     try {
//         c.set_access_channels(websocketpp::log::alevel::all);
//         c.clear_access_channels(websocketpp::log::alevel::frame_payload);
//         c.set_error_channels(websocketpp::log::elevel::all);
//         c.init_asio();

//         c.set_message_handler(&on_message);
//         c.set_tls_init_handler(bind(&on_tls_init, hostname.c_str(), ::_1));

//         c.set_open_handler(bind(&on_open, ::_1, &c));
//         c.set_fail_handler(bind(&on_fail, ::_1));
//         c.set_close_handler(bind(&on_close, ::_1));
//         c.set_error_channels(websocketpp::log::elevel::all);  // Enable detailed error logging
//         websocketpp::lib::error_code ec;
//         client::connection_ptr con = c.get_connection(uri, ec);
//         if (ec) {
//             std::cout << "Could not create connection because: " << ec.message() << std::endl;
//             return 0;
//         }
//         c.connect(con);

//         c.run();
//     } catch (websocketpp::exception const & e) {
//         std::cout << "WebSocket Exception: " << e.what() << std::endl;
//     }
// }

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> // JSON library (https://github.com/nlohmann/json)
#include "WebsocketClient.h"
#include <boost/asio/ssl/context.hpp>

std::shared_ptr<boost::asio::ssl::context> on_tls_init()
{
    auto ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    try
    {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error setting up TLS: " << e.what() << std::endl;
    }
    return ctx;
}

DeribitWebSocketClient::DeribitWebSocketClient()
{
    // Constructor implementation
    c.init_asio();
    c.set_message_handler([this](websocketpp::connection_hdl hdl, client::message_ptr msg)
                          { on_message(hdl, msg); });
    c.set_open_handler([this](websocketpp::connection_hdl hdl)
                       { on_open(hdl); });
    c.set_close_handler([this](websocketpp::connection_hdl hdl)
                        { on_close(hdl); });
}

void DeribitWebSocketClient::connect(const std::string &uri)
{
    c.set_tls_init_handler([this](websocketpp::connection_hdl)
                           { return on_tls_init(); });
    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);
    if (ec)
    {
        std::cerr << "Connection error: " << ec.message() << std::endl;
        return;
    }

    c.connect(con);
    c.run();
}

void DeribitWebSocketClient::on_open(websocketpp::connection_hdl hdl)
{
    std::cout << "Connected to Deribit WebSocket" << std::endl;

    // Subscribe to BTC-USD ticker
    nlohmann::json subscribe_request = {
        {"jsonrpc", "2.0"},
        {"method", "public/subscribe"},
        {"id", 1},
        {"params", {{"channels", {"ticker.BTC-PERPETUAL.raw"}}}}};

    std::string subscribe_message = subscribe_request.dump();
    c.send(hdl, subscribe_message, websocketpp::frame::opcode::text);
    std::cout << "Sent subscription request: " << subscribe_message << std::endl;
}

void DeribitWebSocketClient::on_message(websocketpp::connection_hdl, client::message_ptr msg)
{
    try
    {
        // Parse the received message as JSON
        nlohmann::json response = nlohmann::json::parse(msg->get_payload());
        std::cout << "Received message: " << response.dump(4) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing message: " << e.what() << std::endl;
    }
}

void DeribitWebSocketClient::on_close(websocketpp::connection_hdl)
{
    std::cout << "Connection closed" << std::endl;
}
