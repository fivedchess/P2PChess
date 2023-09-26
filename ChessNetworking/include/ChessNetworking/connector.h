#pragma once
namespace Chess {
  class Package;
  class TimerContainer;
  class Connector {
    public:
      virtual Package& package() = 0;
      virtual TimerContainer& timer() = 0;
      virtual boost::asio::ip::tcp::socket& socket() = 0;
      virtual void run() = 0;
  };
};
