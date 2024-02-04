#ifndef SERVER_H
#define SERVER_H

//create server that communicates using tcp port 8080
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

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;


typedef websocketpp::server<websocketpp::config::asio> server;

class Server {
    private:
        server m_server;
        Database db_;

        json::value handleLogin(json::value logindetails);
        json::value handleRegister(json::value registerdetails);
        json::value getUserDetails(json::value email);
    public:
        Server();
        
        void start(uint16_t port);

        void handleMessage(websocketpp::connection_hdl hdl, server::message_ptr msg);
};

#endif