#pragma once
#include <ChessNetworking/address.h>
namespace Chess {
    class Package;
    class unRouter;
    class Address;
    class Router : public boost::enable_shared_from_this<Chess::Router>{
      protected:
        Address address;
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::endpoint server_endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        void send(Package& package);
        void connectTo(Router router);
        void connectToAll(Router router);
        void do_accept();
        unRouter* to_unRouter();
      public:
        void run();
        Router(Address address);
    };

};
