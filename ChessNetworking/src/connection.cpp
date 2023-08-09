#include <ChessNetworking/connection.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/unrouter.h>
namespace Chess {

  Connection::Connection(boost::asio::ip::tcp::socket socket, Package& package) : socket(std::move(socket)), package(package){
  }
  void Connection::run() {
    auto self(shared_from_this());
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(*package.to->getAddress()), *package.to->getPort());
    (*this->package.data)["From"] = *this->package.from->getAddress() + ":" + std::to_string(*this->package.from->getPort());
    (*this->package.data)["To"] = *this->package.to->getAddress() + ":" + std::to_string(*this->package.to->getPort());
    msgpack::pack(this->msgpack, (*this->package.data));
    socket.async_connect(endpoint, [this, self](boost::system::error_code error) {
      if (!error) {
        boost::asio::async_write(this->socket, boost::asio::buffer(this->msgpack.str()), boost::asio::transfer_all(), [this, self](boost::system::error_code error, std::size_t){
          if (!error) {
            std::cout << "CONNECTION\n";
            this->socket.close();
          } else {
            std::cout << error.message() << std::endl;
            this->socket.close();
          }
        });
      } else {
        std::cout << error.message() << std::endl;
        this->socket.close();
      }
    });
  }
};
