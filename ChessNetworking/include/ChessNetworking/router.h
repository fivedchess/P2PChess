#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
    class Package;
    class Address;
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
        void update(const boost::system::error_code& error);
      public:
        std::unordered_set<unRouter, unRouter_hash>::iterator reseed;
        void send(Package& package, std::function<void(Router*, bool)> callback);
        static const int version;
        const std::unordered_set<unRouter, unRouter_hash>* getConnections();
        const std::string* getAddress() const override;
        const short* getPort() const override;
        void run();
        std::unordered_set<unRouter, unRouter_hash>::iterator connectTo(unRouter router);
        std::unordered_set<unRouter, unRouter_hash>::iterator connectTo(const std::shared_ptr<unRouter>& router);
        std::unordered_set<unRouter, unRouter_hash>::iterator find(const std::shared_ptr<unRouter>& router);
        std::unordered_set<unRouter, unRouter_hash>::iterator find(const unRouter& router);
        std::unordered_set<unRouter, unRouter_hash>::iterator find(const unRouter* router);
        bool inConnections(const std::shared_ptr<unRouter>& router);
        bool inConnections(const unRouter* router);
        bool inConnections(std::unordered_set<unRouter, unRouter_hash>::iterator router);
        void disConnect(const std::shared_ptr<unRouter>& router);
        void disConnect(const unRouter& router);
        void disConnect(std::unordered_set<unRouter, unRouter_hash>::iterator router);
        Router(boost::asio::io_context& io_context, int argc, const char** argv);
    };

};
