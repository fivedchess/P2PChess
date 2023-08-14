#include <ChessNetworking/server_connection.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  ServerConnection::ServerConnection(boost::asio::ip::tcp::socket socket, Router* router) : socket(boost::move(socket)), router(router) {
  }
  void ServerConnection::do_read() {
    auto self(shared_from_this());
    this->socket.async_read_some(boost::asio::buffer(data), [this, self](const boost::system::error_code& error, std::size_t length){
      if (!error) {
        try {
          Request recieved;
          recieved.ParseFromArray(data, length);
          if (recieved.version() != Router::version) {
            std::cout << recieved.version() << std::endl;
            std::cout << "Bad Version" << std::endl;
            this->socket.close();
            return;
          }
          SerialisedConnectable from_router = recieved.from();
          std::cout << "From: " << from_router.address() << ":" << std::to_string(from_router.port()) << std::endl;

          boost::shared_ptr<Request> request = boost::make_shared<Request>();
          request->set_type(Type::Add);
          request->set_version(Router::version);
          if (recieved.type() == Type::Bootstrap) {
            Package package(router, router->connectTo(unRouter(Address(from_router.address(), from_router.port()))), request);
            router->send(package);
          }

        } catch (std::exception) {
          this->socket.close();
          return;
        }
      } else {
        this->socket.close();
        return;
      }
    });
  }
  void ServerConnection::run() {
    this->do_read();
  }
};
