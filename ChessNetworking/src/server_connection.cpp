#include <ChessNetworking/server_connection.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
namespace Chess {
  ServerConnection::ServerConnection(boost::asio::ip::tcp::socket socket, Router* router) : socket(boost::move(socket)), router(router) {
  }
  void ServerConnection::do_read() {
    auto self(shared_from_this());
    this->socket.async_read_some(boost::asio::buffer(data), [this, self](const boost::system::error_code& error, std::size_t length){
      if (!error) {
        try {
          auto tmp = msgpack::unpack(reinterpret_cast<char*>(data), length);
          std::map<std::string, std::string> msgpack;
          tmp.get().convert(msgpack);
          if (msgpack["Ver"] != router->version) {
          std::cout << "Bad Version" << std::endl;
            this->socket.close();
            return;
          }
          std::cout << "From: " <<  msgpack["From"] << std::endl;



          boost::shared_ptr<std::map<std::string, std::string>> data = boost::make_shared<std::map<std::string, std::string>>();
          if (msgpack["Type"] == "Bootstrap") {
            Package package(router, router->connectTo(unRouter(Address(msgpack["From"]))), data);
            router->send(package);
          }
        } catch (msgpack::v1::type_error) {
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
