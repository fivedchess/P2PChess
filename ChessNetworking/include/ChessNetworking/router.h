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
        std::unordered_set<unRouter, unRouter_hash>::iterator reseed;
        boost::asio::io_context& io_context;
        boost::asio::ip::tcp::endpoint server_endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        void do_accept();
        boost::program_options::options_description m_desc {"Allowed options"};
        boost::program_options::variables_map m_vm;
        std::unordered_set<unRouter, unRouter_hash> connections;
      public:
        void send(Package& package);
        static const std::string version;
        const std::string* getAddress() const override;
        const short* getPort() const override;
        void run();
        std::unordered_set<unRouter, unRouter_hash>::iterator connectTo(unRouter router);
        std::unordered_set<unRouter, unRouter_hash>::iterator find(unRouter* router);
        Router(boost::asio::io_context& io_context, int argc, const char** argv);
    };

};
