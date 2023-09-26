#pragma once
#include <ChessNetworking/package.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessSerialisation/request.pb.h>
#include <ChessNetworking/timer.h>
#include <ChessNetworking/connector.h>
#include <ChessNetworking/client.h>
namespace Chess {
  class Package;
  class Router;
  class Socks5Connection : public std::enable_shared_from_this<Socks5Connection>, public Connector {
    protected:
      ProxyServer& proxy;
      //Package
      Package package_;
      //Boost::asio socket;
      boost::asio::ip::tcp::socket socket_;
      //Socks5 Authorization request;
      std::array<std::byte, 3> handshake_request = {std::byte(0x05), std::byte(0x01), std::byte(0x00)};
      //Socks5 Server Response;
      std::array<std::byte, 2> handshake_reply;
      //Connect to server Request;
      std::array<std::byte, 257> connect_request = {std::byte(0x05), std::byte(0x01), std::byte(0x00), std::byte(0x03)};
      //Reply From Socks5 Server for Connect Request;
      std::array<std::byte, 10> connect_reply;
      //Connect to Socks5 Server;
      void proxy_connect() noexcept;
      //Handshake Function;
      void handshake() noexcept;
      //Connect to Router;
      void connect() noexcept;
      //Connect Handler;
      void connect_handler() noexcept;
      //Serialised data;
      std::stringstream msgpack;
      //Deadline Timer
      TimerContainer timer_;
      //Connection Reply
      std::vector<unsigned char> ok;
      //Resolver
      boost::asio::ip::tcp::resolver resolver;
      //Main Client
      Client client;
    public:
      Socks5Connection(boost::asio::ip::tcp::socket socket, ProxyServer& proxy, Package package, TimerContainer timer, boost::asio::ip::tcp::resolver resolver);
      void run() noexcept;
      TimerContainer& timer() noexcept;
      Package& package() noexcept;
      ~Socks5Connection() noexcept;
      boost::asio::ip::tcp::socket& socket() noexcept;
  };
};
