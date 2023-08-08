#pragma once
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/proxy_server.h>
namespace Chess {
  class Socks5Connection {
    protected:
      ProxyServer& proxy;
      Connectable& router;
      //Boost::asio socket;
      boost::asio::ip::tcp::socket socket;
      //Socks5 Authorization request;
      uint8_t handshake_request[3] = {0x05, 0x01, 0x00};
      //Socks5 Server Response;
      uint8_t handshake_reply[2];
      //Connect to server Request;
      std::vector<uint8_t> connect_request;
      //Reply From Socks5 Server for Connect Request;
      uint8_t connect_reply[2];
      //Connect to Socks5 Server;
      void proxy_connect();
      //Handshake Function;
      void handshake();
      //Connect to Router;
      void connect();
      //Connect Handler;
      virtual void connect_handler();
    public:
      Socks5Connection(boost::asio::io_context& io_context, ProxyServer& proxy, Connectable& router);
      void run();
  };
};
