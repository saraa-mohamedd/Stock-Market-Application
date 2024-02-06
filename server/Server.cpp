#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include "Database.h"
#include "Server.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;


Server:: Server() : db_("localhost:3306", "root", "password", "stockmarket"){
    if(!db_.connect())({
        std::cerr << "Failed to connect to the database" << std::endl;
        exit(1);
        });
    else{
        std::cout << "Connected to the database" << std::endl;
    }
}
    
void Server::start(uint16_t port) {
    m_server.init_asio();
    m_server.set_reuse_addr(true);
    m_server.set_message_handler([this](auto hdl, auto msg) {
        this->handleMessage(hdl, msg);
    });
    getStocksInfo();
    m_server.listen(port);
    m_server.start_accept();
    m_server.run();
}

void Server::handleMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {

    if(msg->get_opcode() == websocketpp::frame::opcode::text) {
        std::cout << "Received text message: " << msg->get_payload() << std::endl;
        json::value req = json::value::parse(msg->get_payload());
        json::value response;

        if (req.at("fun").as_string() == "login") {
            response = handleLogin(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "register") {
            response = handleRegister(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "getdetails") {
            response = getUserDetails(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "getallstocks"){
            response = getStocksInfo();
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "getuserstocks"){
            response = getUserStocks(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "buystock"){
            response = buyStock(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "sellstock"){
            response = sellStock(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "gettransactions"){
            response = getUserTransactions(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else{
            std::cout << "Invalid function" << std::endl;
        }
    } else if(msg->get_opcode() == websocketpp::frame::opcode::binary) {
        std::cout << "Received binary message" << std::endl;
    }
}

json::value Server::handleLogin(json::value logindetails) {

    std::string email = logindetails.at("data").at("email").as_string();
    std::string password = logindetails.at("data").at("password").as_string();

    // Check if the email and password match a record in the database
    std::string query = "SELECT * FROM stockmarket.users WHERE email = '" + email + "' AND password = '" + password + "'";
    sql::ResultSet* res;
    json::value response;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        delete res;
        throw e;
    }

    // If a record is found, send the user's details to the client
    if(res->next()) {
        json::value data;
        data[U("fullname")] = json::value::string(U(res->getString("fullName")));
        data[U("email")] = json::value::string(email);

        // Send a success message to the client
        response[U("status")] = json::value::string("success");
        response[U("message")] = json::value::string("Login successful!");
        response[U("fun")] = json::value::string("login");
        response[U("data")] = data;
    } else {
        // Send a failure message to the client
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Invalid email or password. Please try again.");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("login");
    }

    delete res;
    return response;
}

json::value Server::handleRegister(json::value registerdetails) {
    std::string fullname = registerdetails.at("data").at("fullname").as_string();
    std::string email = registerdetails.at("data").at("email").as_string();
    std::string password = registerdetails.at("data").at("password").as_string();

    // Check if the email is already in use
    std::string query = "SELECT * FROM stockmarket.users WHERE email = '" + email + "'";
    sql::ResultSet* res;
    json::value response;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Registration failed. Please try again.");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("register");

        delete res;
        return response;
    }

    // If the email is already in use, send a failure message to the client
    if (res->next()) {
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Email already in use. Please try again.");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("register");
        delete res;
        return response;
    }

    // If the email is not in use, add the user to the database
    query = "INSERT INTO stockmarket.users (fullName, email, password) VALUES ('" + fullname + "', '" + email + "', '" + password + "')";
    try{
        db_.executeQuery(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Registration failed. Please try again.");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("register");

        delete res;
        return response;
    }

    json::value data;
    data[U("fullname")] = json::value::string(fullname);
    data[U("email")] = json::value::string(email);

    response[U("status")] = json::value::string("success");
    response[U("message")] = json::value::string("Registration successful! Please login to continue.");
    response[U("data")] = json::value::object();
    response[U("fun")] = json::value::string("register");

    delete res;
    return response;
}

json::value Server::getUserDetails(json::value email) {
    std::string email_ = email.at("data").at("email").as_string();
    std::string query = "SELECT * FROM stockmarket.users WHERE email = '" + email_ + "'";
    sql::ResultSet* res;
    json::value response;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("User details not found!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("getdetails");
        delete res;
        return response;
    }

    // If a record is found, send the user's details to the client
    if(res->next()) {
        json::value data;
        data[U("fullname")] = json::value::string(U(res->getString("fullName")));
        data[U("email")] = json::value::string(email_);

        response[U("status")] = json::value::string("success");
        response[U("message")] = json::value::string("User details found!");
        response[U("data")] = data;
        response[U("fun")] = json::value::string("getdetails");
    } // If no record is found, send a failure message to the client
    else {
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("User details not found!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("getdetails");
    }

    delete res;
    return response;
}

json::value Server::getStocksInfo(){
    //call api endpoint (from stock service) to get stock info
    http_client client(U("http://localhost:8080/stocks"));
    json::value apiresponse;
    json::value response;

    try{
        http_response res = client.request(methods::GET).get();
        apiresponse = res.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to get stocks info!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("getallstocks");
        return response;
    }

    response[U("status")] = json::value::string("success");
    response[U("message")] = json::value::string("Stocks info found!");
    response[U("data")] = apiresponse;
    response[U("fun")] = json::value::string("getallstocks");

    return response;
}

json::value Server::buyStock(json::value buydetails){
    //call api endpoint (from stock service) to buy stock
    http_client client(U("http://localhost:8080/buystock"));
    json::value apiresponse;
    json::value response;

    try{
        http_request req(methods::POST);
        req.set_body(buydetails.at("data"));

        http_response resp = client.request(req).get();
        apiresponse = resp.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to buy stock!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("buystock");
        return response;
    }
    //edit price string to remove the quotations around it
    std::string pricestr = buydetails.at("data").at("price").as_string().substr(1, buydetails.at("data").at("price").as_string().length()-2);

    //log transaction in database
    std::string query = "INSERT INTO stockmarket.transactions (email, type, stockCompany, price, datetime) VALUES ('" + buydetails.at("data").at("email").as_string() + "', 'buy', '" + buydetails.at("data").at("company").as_string() + "', " + pricestr + ", now())";
    sql::ResultSet* res;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        json::value response;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to log transaction!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("buystock");

        delete res;
        return response;
    }

    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("buystock");

    delete res;
    return response;
}

json::value Server::sellStock(json::value selldetails){
    //call api endpoint (from stock service) to sell stock
    http_client client(U("http://localhost:8080/sellstock"));
    json::value apiresponse;
    json::value response;

    try{
        http_request req(methods::POST);
        req.set_body(selldetails.at("data"));

        http_response res = client.request(req).get();
        apiresponse = res.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to sell stock!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("sellstock");

        delete res;
        return response;
    }
    //edit price string to remove the quotations around it
    std::string pricestr = selldetails.at("data").at("price").as_string().substr(1, selldetails.at("data").at("price").as_string().length()-2);
    
    //log transaction in database
    std::string query = "INSERT INTO stockmarket.transactions (email, type, stockCompany, price, datetime) VALUES ('" + selldetails.at("data").at("email").as_string() + "', 'sell', '" + selldetails.at("data").at("company").as_string() + "', " + pricestr +  ", now())";
    sql::ResultSet* res;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to log transaction!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("sellstock");

        delete res;
        return response;
    }

    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("sellstock");

    delete res;
    return response;
}

json::value Server::getUserStocks(json::value email){
    //call api endpoint (from stock service) to get user stocks with email as query parameter
    http_client client(U("http://localhost:8080/userstocks/?email=" + email.at("data").at("email").as_string()));
    json::value apiresponse;
    json::value response;

    try{
        http_request req(methods::GET);
        http_response res = client.request(req).get();
        apiresponse = res.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to get user stocks!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("getuserstocks");

        delete res;
        return response;
    }

    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("getuserstocks");

    delete res;
    return response;
}

json::value Server::getUserTransactions(json::value email){
    //get user transactions from database
    std::string query = "SELECT * FROM stockmarket.transactions WHERE email = '" + email.at("data").at("email").as_string() + "'";
    sql::ResultSet* res;
    json::value response;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("User transactions not found!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("gettransactions");

        delete res;
        return response;
    }

    //create transactions array of json objects
    response[U("data")][U("transactions")] = json::value::array();

    //add each transaction to the transactions array
    while(res->next()){
        json::value transaction;
        transaction[U("type")] = json::value::string(res->getString("type"));
        transaction[U("stockCompany")] = json::value::string(res->getString("stockCompany"));
        transaction[U("price")] = json::value::number(static_cast<double>(res->getDouble("price")));
        transaction[U("datetime")] = json::value::string(res->getString("datetime"));
        transaction[U("type")] = json::value::string(res->getString("type"));   
        //add transaction to transactions
        response[U("data")][U("transactions")][response[U("data")][U("transactions")].size()] = transaction;
    }

    response[U("status")] = json::value::string("success");
    response[U("message")] = json::value::string("User transactions found!");
    response[U("fun")] = json::value::string("gettransactions");

    delete res;
    return response;
}