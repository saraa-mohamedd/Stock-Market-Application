//create server that communicates using tcp port 8080
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

class Server {
private:
    server m_server;
public:
    Server() {}
    
    void start(uint16_t port) {
        m_server.init_asio();
        m_server.set_reuse_addr(true);
        m_server.set_message_handler([this](auto hdl, auto msg) {
            this->handleMessage(hdl, msg);
        });
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }

    void handleMessage(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        // Handle different types of messages here
        // For example:
        if(msg->get_opcode() == websocketpp::frame::opcode::text) {
            std::cout << "Received text message: " << msg->get_payload() << std::endl;
            //send message back to client
            m_server.send(hdl, msg->get_payload(), msg->get_opcode());
        } else if(msg->get_opcode() == websocketpp::frame::opcode::binary) {
            std::cout << "Received binary message" << std::endl;
        }
    }
};