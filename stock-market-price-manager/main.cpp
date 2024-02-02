#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class StockServer {
public:
    StockServer() : listener_(uri(U("http://localhost:8080"))) {
        listener_.support(methods::GET, std::bind(&StockServer::handle_get, this, std::placeholders::_1));
    }

    void start() {
        listener_.open().wait();
    }

private:
    void handle_get(http_request request) {
        json::value response;
        response[U("message")] = json::value::string(U("Hello, World!"));
        request.reply(http::status_codes::OK, response);
    }

    http_listener listener_;
};

int main() {
    StockServer server;
    server.start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
