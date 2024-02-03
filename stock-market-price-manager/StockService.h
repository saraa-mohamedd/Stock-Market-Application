#ifndef _STOCK_SERVICE_H_
#define _STOCK_SERVICE_H_
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include "Database.cpp"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class StockService {
    public:
        StockService();
        void start();

        //method for periodic stock price update
        void randomUpdateStocks();

    private:
        //database and listener
        Database db_;
        http_listener listener_;

        //methods for interacting with the database
        json::value getStocksInfo();
        json::value buyStock(std::string email, std::string company);
        json::value sellStock(std::string email, std::string company);
        json::value getUserStocks(std::string email);

        //handlers for the HTTP requests
        void handle_get(http_request request);
        void handle_post(http_request request);
};

#endif