#pragma once
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
// cmake -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake ..

class Deribit_api_call
{
public:
    Deribit_api_call();

    std::string authorize(const std::string &clientId, const std::string &clientSecret);

    void placeOrder(const std::string &price, const std::string &amount, const std::string &instrument);

    void cancelOrder(const std::string &orderID);

    void modifyOrder(const std::string &orderID, int amount, double price);

    void getOrderBook(const std::string &instrument);

    void getPosition(const std::string &instrument);

    void getOpenOrders();

private:
    std::string access_token;
    std::string sendRequest_common(const std::string &url, const json &payload);
    // getAccessToken(const std::string& clientId, const std::string& clientSecret);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
};