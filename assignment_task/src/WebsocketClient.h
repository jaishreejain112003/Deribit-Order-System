#pragma once
#include <websocketpp/config/asio_client.hpp> 
#include <websocketpp/client.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp> 



typedef websocketpp::client<websocketpp::config::asio_tls_client> client;



class DeribitWebSocketClient {
public:
    DeribitWebSocketClient();

    void connect(const std::string &uri);

private:
    client c;

    void on_open(websocketpp::connection_hdl hdl);

    void on_message(websocketpp::connection_hdl , client::message_ptr msg);

    void on_close(websocketpp::connection_hdl);
    
};


