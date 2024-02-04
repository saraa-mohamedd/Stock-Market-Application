#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <cpprest/http_listener.h>
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
using namespace web::http::experimental::listener;


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
    m_server.listen(port);
    m_server.start_accept();
    m_server.run();
}

void Server::handleMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {

    if(msg->get_opcode() == websocketpp::frame::opcode::text) {
        std::cout << "Received text message: " << msg->get_payload() << std::endl;
        json::value req = json::value::parse(msg->get_payload());

        if (req.at("fun").as_string() == "login") {
            json::value response = handleLogin(req);
             m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "register") {
            json::value response = handleRegister(req);
            m_server.send(hdl, response.serialize(), msg->get_opcode());
        }
        else if (req.at("fun").as_string() == "getdetails") {
            json::value response = getUserDetails(req);
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
        response[U("message")] = json::value::string("Email already exists. Please try again.");
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
    response[U("message")] = json::value::string("Registration successful!");
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