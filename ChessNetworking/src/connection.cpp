#include <ChessNetworking/connection.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/unrouter.h>
namespace Chess {

  Connection::Connection(boost::asio::ip::tcp::socket socket, Package& package) : socket(std::move(socket)), package(package){
  }
  void Connection::run() {
    auto self(shared_from_this());
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(*package.to->getAddress()), *package.to->getPort());
    (*package.data)["From"] = *package.from->getAddress() + ":" + std::to_string(*package.from->getPort());
    (*package.data)["To"] = *package.to->getAddress() + ":" + std::to_string(*package.to->getPort());
    msgpack::pack(msgpack, (*package.data));
    socket.async_connect(endpoint, [this, self](boost::system::error_code error) {
      if (!error) {
        boost::asio::async_write(socket, boost::asio::buffer(msgpack.str()), [this, self](boost::system::error_code error, std::size_t){
          std::cout << "CONNECTION\n";
          socket.close();
        });
      } else {
        std::cout << error.message() << std::endl;
      }
    });
  }
};
