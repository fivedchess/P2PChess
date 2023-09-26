#pragma once
#include <ChessNetworking/package.h>
#include <ChessNetworking/timer.h>
namespace Chess {
  class Package;
  class Connector;
  class Client {
    protected:
      Package& package;
      boost::asio::ip::tcp::socket& socket;
      TimerContainer& timer;
      std::array<std::byte, 20> reply;
    public:
      Client(Connector* connector);
      void run(const std::shared_ptr<Connector> connector);
  };
};
