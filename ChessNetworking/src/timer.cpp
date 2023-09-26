#include <ChessNetworking/timer.h>
namespace Chess {
  TimerContainer::TimerContainer(boost::asio::steady_timer timer, std::function<void(bool)> callback, bool status) :
      timer(std::move(timer)),
      status(std::move(status)),
      callback(std::move(callback)) {};
  TimerContainer::TimerContainer(TimerContainer&& container) :
      timer(std::move(container.timer)),
      status(std::move(container.status)),
      callback(std::move(container.callback)){};
};
