#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
    class Package;
    class unRouter;
    class Address;
    class Router : public boost::enable_shared_from_this<Chess::Router>, public Connectable{
      protected:
        Address address;
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::endpoint server_endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        void send(Package& package);
        void connectTo(unRouter router);
        void do_accept();
      public:
        std::string* getAddress() override;
        short* getPort() override;
        void run();
        Router(Address address);
    };

};
