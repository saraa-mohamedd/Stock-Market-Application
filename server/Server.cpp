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

    std::string query = "SELECT * FROM stockmarket.users WHERE email = '" + email + "' AND password = '" + password + "'";
    sql::ResultSet* res;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    json::value response;

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


        std::cout << response << std::endl;
    }
    delete res;
    std:: cout << "serialized response" << response.serialize() << std::endl;
    return response;
}

json::value Server::handleRegister(json::value registerdetails) {
    std::string fullname = registerdetails.at("data").at("fullname").as_string();
    std::string email = registerdetails.at("data").at("email").as_string();
    std::string password = registerdetails.at("data").at("password").as_string();

    std::string query = "SELECT * FROM stockmarket.users WHERE email = '" + email + "'";
    sql::ResultSet* res;

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    if (res->next()) {
        json::value response;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Email already in use. Please try again.");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("register");
        delete res;
        return response;
    }

    query = "INSERT INTO stockmarket.users (fullName, email, password) VALUES ('" + fullname + "', '" + email + "', '" + password + "')";
    try{
        db_.executeQuery(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    json::value data;
    data[U("fullname")] = json::value::string(fullname);
    data[U("email")] = json::value::string(email);

    json::value response;
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

    try{
        res = db_.executeSelect(query);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    json::value response;
    if(res->next()) {
        json::value data;
        data[U("fullname")] = json::value::string(U(res->getString("fullName")));
        data[U("email")] = json::value::string(email_);

        response[U("status")] = json::value::string("success");
        response[U("message")] = json::value::string("User details found!");
        response[U("data")] = data;
        response[U("fun")] = json::value::string("getdetails");
    } else {
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("User details not found!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("getdetails");
    }
    delete res;
    return response;
}

json::value Server::getStocksInfo(){
    //call api to get stock info
    http_client client(U("http://localhost:8080/stocks"));
    json::value apiresponse;
    try{
        http_response res = client.request(methods::GET).get();
        apiresponse = res.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        throw e;
    }

    json::value response;
    response[U("status")] = json::value::string("success");
    response[U("message")] = json::value::string("Stocks info found!");
    response[U("data")] = apiresponse;
    response[U("fun")] = json::value::string("getallstocks");


    // std::cout << "Stocks info: " << response << std::endl;
    return response;
}

json::value Server::buyStock(json::value buydetails){
    http_client client(U("http://localhost:8080/buystock"));
    json::value apiresponse;
    try{
        http_request req(methods::POST);
        req.set_body(buydetails.at("data"));

        http_response resp = client.request(req).get();
        apiresponse = resp.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        json::value response;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to buy stock!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("buystock");
        return response;
    }

    json::value response;
    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("buystock");

    return response;
}

json::value Server::sellStock(json::value selldetails){
    http_client client(U("http://localhost:8080/sellstock"));
    json::value apiresponse;
    try{
        http_request req(methods::POST);
        req.set_body(selldetails.at("data"));

        http_response res = client.request(req).get();
        apiresponse = res.extract_json().get();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        json::value response;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to sell stock!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("sellstock");
        return response;
    }

    json::value response;
    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("sellstock");

    return response;
}

json::value Server::getUserStocks(json::value email){
    http_client client(U("http://localhost:8080/userstocks/?email=" + email.at("data").at("email").as_string()));
    json::value apiresponse;

    try{
        http_request req(methods::GET);
        // req.set_request_uri(U("/endpoint?param1=value1&param2=value2"));
        // req.headers().add(U("email"), email.at("data").at("email").as_string());
        std::cout << "Request at TCP: " << req.to_string() << std::endl;
        http_response res = client.request(req).get();

        try{
            apiresponse = res.extract_json().get();
        }
        catch(const std::exception& e){
            json::value response;
            response[U("status")] = json::value::string("failure");
            response[U("message")] = json::value::string("Failed to sell stock!");
            response[U("data")] = json::value::object();
            response[U("fun")] = json::value::string("sellstock");
            return response;
        }
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        json::value response;
        response[U("status")] = json::value::string("failure");
        response[U("message")] = json::value::string("Failed to sell stock!");
        response[U("data")] = json::value::object();
        response[U("fun")] = json::value::string("sellstock");
        return response;
    }

    json::value response;
    response[U("status")] = json::value::string("success");
    response[U("message")] = apiresponse.at("message");
    response[U("data")][U("stocks")] = apiresponse.at("stocks");
    response[U("fun")] = json::value::string("getuserstocks");

    return response;

}