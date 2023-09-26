#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/unrouter_hash.h>
class Request;
class SerialisedConnectable;
namespace Chess {
    class Package;
    class Address;
    class Connectable_hash;
    class ServerSession;
    class Router : public boost::enable_shared_from_this<Chess::Router>, public Connectable{
      protected:
        Address address;
        ProxyServer proxy;
        boost::asio::io_context& io_context;
        boost::asio::ip::tcp::endpoint server_endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        void do_accept();
        boost::program_options::options_description m_desc {"Allowed options"};
        boost::program_options::variables_map m_vm;
        std::unordered_set<unRouter, unRouter_hash> connections;
        void reseed_connection();
        const void update();
        const int version_;
        boost::asio::steady_timer updator_timer;
        void connectionAccept(std::shared_ptr<ServerSession> connection);
        void addRouters(Request& request);
        void bootstrapConnection(Request request, SerialisedConnectable from);
        void addConnection(Request request, Request recieved);
      public:
        const Connectable* reseed;
        void send(Package package, std::function<void(bool)> callback);
        const std::string& getAddress() const override;
        const short& getPort() const override;
        const int version() const override;
        void run();
        const unRouter& connectTo(unRouter router);
        bool inConnections(const unRouter& router);
        void disConnect(const unRouter& router);
        void forConnections(std::function<void(const unRouter&)> callback);
        Router(boost::asio::io_context& io_context, int argc, const char** argv);
    };

};
