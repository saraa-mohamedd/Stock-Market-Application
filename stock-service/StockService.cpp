#pragma once

#include "StockService.h"

StockService::StockService() : listener_(uri(U("http://localhost:8080"))), db_("localhost:3306", "root", "password", "stockmarket") {
    listener_.support(methods::GET, std::bind(&StockService::handle_get, this, std::placeholders::_1));
    listener_.support(methods::POST, std::bind(&StockService::handle_post, this, std::placeholders::_1));

    if(!db_.connect())({
        std::cerr << "Failed to connect to the database" << std::endl;
        exit(1);
        });
    else{
        std::cout << "Connected to the database" << std::endl;
    }
}

void StockService::start() {
    listener_.open().wait();
}

void StockService::handle_get(http_request request) {
    auto path = request.relative_uri().path();
    auto params = request.absolute_uri().split_query(request.relative_uri().query());
    json::value response;

    if (path == U("/")) {
        response[U("message")] = json::value::string(U("Ping!"));
        request.reply(http::status_codes::OK, response);
    }
    else if (path == U("/stocks")) {
        try{
            response = getStocksInfo();
            request.reply(http::status_codes::OK, response);
        }
        catch(const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::ExpectationFailed, e.what());
        }
        catch (const std::exception& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::RequestTimeout);
        }
    }
    else if (path == U("/userstocks")) {
        try{
            json::value response;
            std::string email = params["email"];
            response = getUserStocks(email);
            request.reply(http::status_codes::OK, response);
        }
        catch(const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::ExpectationFailed, e.what());
        }
    }
    else {
        request.reply(http::status_codes::NotFound);
    }
}

void StockService::handle_post(http_request request) {
    auto path = request.relative_uri().path();
    json::value jsonBody;

    if (path==U("/buystock")){
        request.extract_json().then([this, request](json::value jvalue) {
            try{
            json::value response;
            response = buyStock(jvalue[U("email")].as_string(), jvalue[U("company")].as_string());
            request.reply(http::status_codes::OK, response);
        }
        catch(const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::ExpectationFailed, e.what());
        }
        });
    }
    else if (path==U("/sellstock")){
        request.extract_json().then([this, request](json::value jvalue) {
            try{
            json::value response;
            response = sellStock(jvalue[U("email")].as_string(), jvalue[U("company")].as_string());
            request.reply(http::status_codes::OK, response);
        }
        catch(const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::ExpectationFailed, e.what());
        }
        });
    }
    else{
        request.reply(http::status_codes::NotFound);
    }
}

void StockService::handle_delete(http_request request) {
    auto path = request.relative_uri().path();
    auto params = request.absolute_uri().split_query(request.relative_uri().query());
    json::value response;

    if (path == U("/sellstock")) {
        request.extract_json().then([this, request](json::value jvalue) {
            try{
            json::value response;
            response = sellStock(jvalue[U("email")].as_string(), jvalue[U("company")].as_string());
            request.reply(http::status_codes::OK, response);
        }
        catch(const std::runtime_error& e){
            std::cerr << e.what() << std::endl;
            request.reply(http::status_codes::ExpectationFailed, e.what());
        }
        });
    }
    else {
        request.reply(http::status_codes::NotFound);
    }
}

json::value StockService::getStocksInfo(){
    std::string sqlQuery = "SELECT * FROM stockmarket.stocks";
    sql::ResultSet* res = db_.executeSelect(sqlQuery);
    
    json::value response;
    response[U("stocks")] = json::value::array();

    while (res->next()) {
        json::value stock;
        stock[U("company")] = json::value::string(U(res->getString("company")));
        stock[U("price")] = json::value::string(U(res->getString("price")));
        stock[U("remainingShares")] = json::value::string(U(res->getString("remainingShares")));
        stock[U("percentPerSell")] = json::value::string(U(res->getString("percentPerSell")));
        stock[U("percentPerBuy")] = json::value::string(U(res->getString("percentPerBuy")));
        response[U("stocks")][response[U("stocks")].size()] = stock;
    }

    std::cout << response.serialize() << std::endl;
    return response;
}

json::value StockService::buyStock(std::string email, std::string company){
    std::string sqlQuery = "SELECT * FROM stockmarket.stocks WHERE company = '" + company + "'";
    sql::ResultSet* res = db_.executeSelect(sqlQuery);
    json::value response;

    if (res->next()){
        int remainingShares = res->getInt("remainingShares");
        if (remainingShares > 0){
            double price = res->getDouble("price");
            double percentPerBuy = res->getDouble("percentPerBuy");
            double newPrice = price + (price * 0.01 * percentPerBuy);
            remainingShares--;

            sqlQuery = "INSERT INTO stockmarket.user_stocks (email, stockCompany, priceBought) VALUES ('" + email + "', '" + company + "', " + std::to_string(price) + ")";

            try{
                db_.executeQuery(sqlQuery);
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
                response[U("message")] = json::value::string(U("Error buying stock"));
                return response;
            }

            sqlQuery = "UPDATE stockmarket.stocks SET price = " + std::to_string(newPrice) + ", remainingShares = " + std::to_string(remainingShares) + " WHERE company = '" + company + "'";
            try{
                db_.executeQuery(sqlQuery);
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
                response[U("message")] = json::value::string(U("Error buying stock"));
                return response;
            }

            response[U("message")] = json::value::string(U("Stock bought successfully"));
        }
        else{
            response[U("message")] = json::value::string(U("No remaining shares"));
        }
    }
    else{
        response[U("message")] = json::value::string(U("Company not found"));
    }

    response[U("stocks")] = getUserStocks(email)[U("stocks")];

    return response;
}

json::value StockService::sellStock(std::string email, std::string company){
    std::string sqlQuery = "SELECT * FROM stockmarket.user_stocks WHERE email = '" + email + "' AND stockCompany = '" + company + "'";
    sql::ResultSet* res = db_.executeSelect(sqlQuery);
    json::value response;

    if (res->next()){
        double priceBought = res->getDouble("priceBought");
        sqlQuery = "WITH CTE AS(SELECT stockId from stockmarket.user_stocks WHERE email= '" + email + "' AND stockCompany='" + company + "' ORDER BY priceBought ASC LIMIT 1) DELETE FROM stockmarket.user_stocks WHERE stockId IN (SELECT stockId FROM CTE)";
        try{
            db_.executeQuery(sqlQuery);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
            response[U("message")] = json::value::string(U("Error selling stock"));
            return response;
        }

        sqlQuery = "SELECT * FROM stockmarket.stocks WHERE company = '" + company + "'";
        res = db_.executeSelect(sqlQuery);
        if (res->next()){
            double price = res->getDouble("price");
            double percentPerSell = res->getDouble("percentPerSell");
            double newPrice = price - (price * 0.01 * percentPerSell);
            int remainingShares = res->getInt("remainingShares");
            remainingShares++;

            sqlQuery = "UPDATE stockmarket.stocks SET price = " + std::to_string(newPrice) + ", remainingShares = " + std::to_string(remainingShares) + " WHERE company = '" + company + "'";
            try{
                db_.executeQuery(sqlQuery);
            }
            catch(const std::exception& e){
                std::cerr << e.what() << std::endl;
                response[U("message")] = json::value::string(U("Error selling stock"));
                return response;
            }

            response[U("message")] = json::value::string(U("Stock sold successfully"));
        }
        else{
            response[U("message")] = json::value::string(U("Company not found"));
        }
    }
    else{
        response[U("message")] = json::value::string(U("Stock not found"));
    }

    response[U("stocks")] = getUserStocks(email)[U("stocks")];

    return response;
}

json::value StockService::getUserStocks(std::string email){
    std::string sqlQuery = "SELECT * FROM stockmarket.user_stocks WHERE email = '" + email + "'";
    sql::ResultSet* res;

    try{
        res = db_.executeSelect(sqlQuery);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }
    json::value response;
    response[U("stocks")] = json::value::array();

    while (res->next()) {
        json::value stock;
        stock[U("company")] = json::value::string(U(res->getString("stockCompany")));
        stock[U("priceBought")] = json::value::string(U(res->getString("priceBought")));
        response[U("stocks")][response[U("stocks")].size()] = stock;
    }

    return response;
}

void StockService::randomUpdateStocks(){

    std::string sqlQuery = "SELECT * FROM stockmarket.stocks";
    sql::ResultSet* res;
    
    try{
        res = db_.executeSelect(sqlQuery);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    while (res->next()){
        std::string company = res->getString("company");
        double price = res->getDouble("price");

        double random = ((double) rand() / (RAND_MAX)) * 0.2 - 0.1;
        price = price + price * random;
        std::stringstream s;
        s << std::fixed << std::setprecision(3) << price;
        price = std::stod(s.str());

        sqlQuery = "UPDATE stockmarket.stocks SET price = " + std::to_string(price) + " WHERE company = '" + company + "'";
        try{
            db_.executeQuery(sqlQuery);
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
        }
    }
}

// g++ -Wall -I/usr/include/cppconn main.cpp -lcpprest -lssl -lcrypto -L/usr/lib -lmysqlcppconn