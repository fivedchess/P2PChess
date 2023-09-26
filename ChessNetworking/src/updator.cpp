#include <ChessNetworking/updator.h>
#include <ChessNetworking/package.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  Updator::Updator(boost::asio::io_context& io_context, Router& router) :
  io_context(io_context),
  router(router) {}
  void Updator::run() {
    /*
    std::cout << "UPDATE!" << std::endl;
    for (const auto& element : this->router.getConnections()){
      std::cout << "Updating: "
        << element.getAddress()
        << ":" << element.getPort()
      << std::endl;
      Request request;
      request.set_type(Type::Bootstrap);
      request.set_version(this->router.version());
      Package package(router, element, std::move(request));
      this->router.send(std::move(package), [this, &element](bool timeout){
        if (!router.inConnections(element)) {
          std::cout << "WARN: " << "Already Deleted" << std::endl;
          return;
        }
        if (!timeout) {
          std::cout
          << "Success: "
          << element.getAddress()
          << ":" << element.getPort()
          << std::endl;
          return;
        }
        std::cout << "Failed: "
        << element.getAddress()
        << ":" << element.getPort()
        << std::endl;
        this->router.disConnect(element);
        return;
      });
    }
    auto timer = new boost::asio::steady_timer(io_context, std::chrono::minutes(5));
    timer->async_wait([this, timer](const boost::system::error_code& error){
      delete timer;
      this->run();
    });
    */
  }
};
