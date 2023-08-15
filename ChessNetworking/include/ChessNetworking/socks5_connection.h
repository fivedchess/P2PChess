#pragma once
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  class Package;
  class Router;
  class Socks5Connection : public boost::enable_shared_from_this<Socks5Connection> {
    protected:
      ProxyServer& proxy;
      std::unordered_set<unRouter, unRouter_hash>::iterator router;
      //Boost::asio socket;
      boost::asio::ip::tcp::socket socket;
      //Socks5 Authorization request;
      uint8_t handshake_request[3] = {0x05, 0x01, 0x00};
      //Socks5 Server Response;
      uint8_t handshake_reply[2];
      //Connect to server Request;
      std::vector<uint8_t> connect_request = {0x05, 0x01, 0x00, 0x03};
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
      //Serialised data;
      std::stringstream msgpack;
      //Not serialised data;
      boost::shared_ptr<Request> data;
      //From Router;
      Router* from;
      //Deadline Timer
      boost::asio::steady_timer timer;
      //Callback
      std::function<void(Router*, bool)> callback;
      //Connection Reply
      std::vector<unsigned char> ok;
      //Timer status
      // true = error
      // false = success
      bool timer_status;
    public:
      Socks5Connection(boost::asio::ip::tcp::socket socket, ProxyServer& proxy, Package& package, boost::asio::steady_timer timer, const std::function<void(Router*, bool)> callback);
      void run();
  };
};
