#pragma once
namespace Chess {
  class TimerContainer {
    public:
      boost::asio::ip::tcp::socket* socket;
      boost::asio::steady_timer timer;
      bool status;
      std::function<void(bool)> callback;
      TimerContainer(TimerContainer&) = delete;
      TimerContainer(TimerContainer&&);
      TimerContainer(boost::asio::steady_timer timer, std::function<void(bool)> callback, bool status);
  };
};
