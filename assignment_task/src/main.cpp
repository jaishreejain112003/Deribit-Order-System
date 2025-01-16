

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <direct.h>
#include "Deribit.h"

using json = nlohmann::json;

int main()
{

    // Obtaining the private credentials via the file config.json

    // std::ifstream file("../../config.json");
    // if (!file.is_open()) {
    //     perror("Error opening file");

    //     return 1;
    // }

    // json j;
    // file >> j;  // Parse the file into a json object

    // std::string clientId = j["clientId"];
    // std::string clientSecret = j["clientSecret"];

    // Websocket implemenation

    // DeribitWebSocketClient webclient;
    // std::string uri = "wss://www.deribit.com/ws/api/v2";
    // webclient.connect(uri);

    Deribit_api_call deribit_client;
    int count = -1;

    while (count)
    {
        std ::cout << "\n";
        std ::cout << "........................ENTERING......................\n";
        std ::cout << "Enter: 1" << " for authorization : " << "\n";
        std ::cout << "Enter: 2" << " for placing the order :" << "\n";
        std ::cout << "Enter: 3" << " for canceling the order :" << "\n";
        std ::cout << "Enter: 4" << " for modifying the order :" << "\n";
        std ::cout << "Enter: 5" << " for getting the OrderBook :" << "\n";
        std ::cout << "Enter: 6" << " for getting the Open Orders :" << "\n";
        std ::cout << "Enter: 7" << " for getting the Position :" << "\n";
        std ::cout << "Enter: 0" << " for exiting :" << "\n";
        std ::cout << "\n";
        std ::cin >> count;

        if (count == 1)
        {
            // for authorization
            std::string clientid;
            std::string clientSecret;
            std ::cout << "Enter the client ID :" << "\n";
            std ::cin >> clientid;
            std ::cout << "Enter the clientSecret :" << "\n";
            std ::cin >> clientSecret;
            std ::cout << "The access token is : " << deribit_client.authorize(clientid, clientSecret) << "\n";
        }
        else if (count == 2)
        {
            // for placing the order
            std::string price;
            std::string amount;
            std::string instrument;
            std ::cout << "Enter the price :" << "\n";
            std ::cin >> price;
            std ::cout << "Enter the amount :" << "\n";
            std ::cin >> amount;
            std ::cout << "Enter the instrument :" << "\n";
            std ::cin >> instrument;
            deribit_client.placeOrder(price, amount, instrument);
        }
        else if (count == 3)
        {
            // for canceling the order
            std::string order_id;
            std ::cout << "Enter the order id :" << "\n";
            std ::cin >> order_id;
            deribit_client.cancelOrder(order_id);
        }
        else if (count == 4)
        {
            // for modifying the order
            double price;
            int amount;
            std::string order_id;
            std ::cout << "Enter the order id:" << "\n";
            std ::cin >> order_id;
            std ::cout << "Enter the amount :" << "\n";
            std ::cin >> amount;
            std ::cout << "Enter the price :" << "\n";
            std ::cin >> price;
            deribit_client.modifyOrder(order_id, amount, price);
        }
        else if (count == 5)
        {
            // for getting the OrderBook
            std::string instrument;
            std ::cout << "Enter the instrument :" << "\n";
            std ::cin >> instrument;
            deribit_client.getOrderBook(instrument);
        }
        else if (count == 6)
        {
            // for getting the Open Orders
            std::string instrument;
            std ::cout << "Enter the instrument :" << "\n";
            std ::cin >> instrument;
            deribit_client.getPosition(instrument);
        }
        else if (count == 7)
        {
            // for getting the Position
            deribit_client.getOpenOrders();
        }
        else if (count == 0)
        {
            // for exiting
            std ::cout << "........................EXITING......................";
        }
        else
        {
            std ::cout << "You entered an incorrect number" << "\n";
            std ::cout << "EXITING......................";
            count = 0;
        }
    }

    return 0;
}