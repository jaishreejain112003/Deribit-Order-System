
#include "Deribit.h"

using json = nlohmann::json;
Deribit_api_call ::Deribit_api_call()
{
    access_token = "";
}
// Function to handle the response from the cURL request
size_t Deribit_api_call ::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// General function to send a cURL request with optional access token
std::string Deribit_api_call ::sendRequest_common(const std::string &url, const json &payload)
{
    std::string readBuffer;
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // Set the HTTP method to POST

        // Set the request payload
        std::string jsonStr = payload.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

        // Set headers, including Authorization if accessToken is provided
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!access_token.empty())
        {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up the write callback to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Free Resources
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer;
}

// Function to get the access token
std::string Deribit_api_call ::authorize(const std::string &clientId, const std::string &clientSecret)
{
    json payload = {
        {"id", 0},
        {"method", "public/auth"},
        {"params", {{"grant_type", "client_credentials"}, {"client_id", clientId}, {"client_secret", clientSecret}}},
        {"jsonrpc", "2.0"}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/public/auth", payload);
    auto responseJson = json::parse(response);

    // Retrieve the access token from the response
    if (responseJson.contains("result") && responseJson["result"].contains("access_token"))
    {
        access_token = responseJson["result"]["access_token"];
        return responseJson["result"]["access_token"];
    }
    else
    {
        std::cerr << "Failed to retrieve access token." << std::endl;
        return "";
    }
}

// Function to place an order
void Deribit_api_call ::placeOrder(const std::string &price, const std::string &amount, const std::string &instrument)
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/buy"},
        {"params", {{"instrument_name", instrument}, {"type", "limit"}, {"price", price}, {"amount", amount}}},
        {"id", 1}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/private/buy", payload);
    std::cout << "Place Order Response: " << response << std::endl;
    auto responseJson = json::parse(response);
    std::cout << "\n";
    std::cout << "Order Id : " << responseJson["result"]["order"]["order_id"] << ", Order Price : " << responseJson["result"]["order"]["price"] << '\n';
    std::cout << "instrument_name : " << responseJson["result"]["order"]["instrument_name"] << ", Amount: " << responseJson["result"]["order"]["amount"] << '\n';
    std::cout << "\n";
}

// Function to cancel an order
void Deribit_api_call ::cancelOrder(const std::string &orderID)
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/cancel"},
        {"params", {{"order_id", orderID}}},
        {"id", 6}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/private/cancel", payload);
    std::cout << "Cancel Order Response: " << response << std::endl;
    std::cout << "\n";
    auto responseJson = json::parse(response);
    std::cout << "Order Id : " << responseJson["result"]["order_id"] << '\n';
    std::cout << "instrument_name : " << responseJson["result"]["instrument_name"] << ", Amount: " << responseJson["result"]["amount"] << '\n';
    std::cout << "\n";
}

// Function to modify an order
void Deribit_api_call ::modifyOrder(const std::string &orderID, int amount, double price)
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/edit"},
        {"params", {{"order_id", orderID}, {"amount", amount}, {"price", price}}},
        {"id", 11}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/private/edit", payload);
    std::cout << "Modify Order Response: " << response << std::endl;
    auto responseJson = json::parse(response);

    if (responseJson.contains("error"))
    {
        std::cout << "Error: " << responseJson["error"]["message"] << std::endl;
        return;
    }

    // Accessing the result
    if (responseJson.contains("result") && responseJson["result"].contains("order"))
    {
        auto order = responseJson["result"]["order"];

        // Safely accessing the values
        std::cout << "Order Id: " << order.value("order_id", "Unknown") << '\n';
        std::cout << "Instrument Name: " << order.value("instrument_name", "Unknown") << '\n';
        std::cout << "Amount: " << order.value("amount", 0) << '\n';
    }
    else
    {
        std::cout << "No order data found in the response." << std::endl;
    }
    // std::cout << "\n";
    // std::cout << "Order Id : " << responseJson["result"]["order"]["order_id"]  << '\n';
    // std::cout << "instrument_name : " << responseJson["result"]["order"]["instrument_name"] << ", Amount: " << responseJson["result"]["order"]["amount"] << '\n';
    // std::cout << "\n";
}

// Function to retrieve the order book
void Deribit_api_call ::getOrderBook(const std::string &instrument)
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "public/get_order_book"},
        {"params", {{"instrument_name", instrument}}},
        {"id", 15}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/public/get_order_book", payload);

    auto responseJson = json::parse(response);
    std::cout << "\n";
    std::cout << "Order Book for " << instrument << ":\n\n";

    // Printing best bid and ask
    std::cout << "Best Bid Price: " << responseJson["result"]["best_bid_price"] << ", Amount: " << responseJson["result"]["best_bid_amount"] << '\n';

    // Printing bids and asks in detail
    std::cout << "Asks:\n";
    for (const auto &ask : responseJson["result"]["asks"])
    {
        std::cout << "Price: " << ask[0] << ", Amount: " << ask[1] << '\n';
    }

    std::cout << "\nBids:\n";
    for (const auto &bid : responseJson["result"]["bids"])
    {
        std::cout << "Price: " << bid[0] << ", Amount: " << bid[1] << '\n';
    }

    // Additional information
    std::cout << "\nMark Price: " << responseJson["result"]["mark_price"] << '\n';
    std::cout << "Timestamp: " << responseJson["result"]["timestamp"] << '\n';
}

// Function to get position details of a specific instrument
void Deribit_api_call ::getPosition(const std::string &instrument)
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/get_position"},
        {"params", {{"instrument_name", instrument}}},
        {"id", 20}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/private/get_position", payload);
    auto responseJson = json::parse(response);

    // Parse and print position details if available
    if (responseJson.contains("result"))
    {
        std::cout << "Position Details for " << instrument << ":\n\n";
        auto result = responseJson["result"];

        std::cout << "Total Profit Loss: " << result["total_profit_loss"] << '\n';

        std::cout << "Average Price: " << result["average_price"] << '\n';

        std::cout << "Initial Margin: " << result["initial_margin"] << '\n';

        std::cout << "Maintenance Margin: " << result["maintenance_margin"] << '\n';

        std::cout << "Size: " << result["size"] << '\n';
    }
    else
    {
        std::cerr << "Error: Could not retrieve position data." << std::endl;
    }
}

// Function to print all open orders with instrument, order ID, price, and amount
void Deribit_api_call ::getOpenOrders()
{
    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/get_open_orders"},
        {"params", {{"kind", "future"}, {"type", "limit"}}},
        {"id", 25}};

    std::string response = sendRequest_common("https://test.deribit.com/api/v2/private/get_open_orders", payload);
    auto responseJson = json::parse(response);

    // Check if the response contains the "result" array
    if (responseJson.contains("result"))
    {
        std::cout << "Open Orders:\n\n";
        for (const auto &order : responseJson["result"])
        {
            std::string instrument = order["instrument_name"];
            std::string orderId = order["order_id"];
            double price = order["price"];
            double amount = order["amount"];

            std::cout << "Instrument: " << instrument << ", Order ID: " << orderId
                      << ", Price: " << price << ", Amount: " << amount << '\n';
        }
    }
    else
    {
        std::cerr << "Error: Could not retrieve open orders." << std::endl;
    }
}