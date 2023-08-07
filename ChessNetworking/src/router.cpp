#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/connection.h>
namespace Chess{
  void Router::do_accept(){
    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
          do_accept();
        });
  }
  unRouter* Router::to_unRouter(){
    return new unRouter(this->address);
  }
  Router::Router(Address address) : address(address), server_endpoint(boost::asio::ip::tcp::v6(), address.port), acceptor(io_context, server_endpoint){}
  void Router::run(){
    do_accept();
    boost::shared_ptr<std::map<std::string, std::string>> Data = boost::make_shared<std::map<std::string, std::string>>();
    (*Data)["Type"] = "Bootstrap";
    Package package(to_unRouter(), new unRouter(Address("127.0.0.1:6002")), Data);
    Connection myConnection(&io_context, package);
    io_context.run();
  }

};

