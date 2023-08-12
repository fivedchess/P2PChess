#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
    class Package;
    class Address;
    class Router : public boost::enable_shared_from_this<Chess::Router>, public Connectable{
      protected:
        Address address;
        ProxyServer proxy;
        unRouter reseed;
        boost::asio::io_context& io_context;
        boost::asio::ip::tcp::endpoint server_endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        void send(Package& package);
        void connectTo(unRouter router);
        void do_accept();
        boost::program_options::options_description m_desc {"Allowed options"};
        boost::program_options::variables_map m_vm;
      public:
        std::string* getAddress() override;
        short* getPort() override;
        void run();
        Router(boost::asio::io_context& io_context, int argc, const char** argv);
    };

};
