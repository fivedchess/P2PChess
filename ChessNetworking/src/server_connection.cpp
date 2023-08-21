#include <ChessNetworking/server_connection.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  ServerConnection::ServerConnection(boost::asio::ip::tcp::socket socket, Router* router) : socket(boost::move(socket)), router(router) {
  }
  void ServerConnection::add_routers(SerialisedConnectable& from_router, boost::shared_ptr<Request> request) {
    for (auto node : *router->getConnections()) {
      if (*node.getAddress() != from_router.address() || *node.getPort() != from_router.port()){
        SerialisedConnectable* myRouter = request->add_connections();
        myRouter->set_address(*node.getAddress());
        myRouter->set_port(*node.getPort());
        std::cout << "Router Added To request: " << *node.getAddress() << ":" << *node.getPort() << std::endl;
      }
    }
  }
  void ServerConnection::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read(this->socket, boost::asio::dynamic_buffer(data), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t length){
      if (!error) {
        try {
          Request recieved;
          recieved.ParseFromArray(data.data(), length);
          if (recieved.version() != Router::version) {
            std::cout << recieved.version() << std::endl;
            std::cout << "Bad Version" << std::endl;
            this->socket.close();
            return;
          }
          SerialisedConnectable from_router = recieved.from();
          std::cout << "From: " << from_router.address() << ":" << std::to_string(from_router.port()) << std::endl;
          boost::shared_ptr<Request> request = boost::make_shared<Request>();
          request->set_version(router->version);

          auto tmp = new Request;
          tmp->set_type(Type::Success);
          tmp->set_version(Router::version);
          boost::asio::async_write(this->socket, boost::asio::buffer(tmp->SerializeAsString()), boost::asio::transfer_all(), [tmp](const boost::system::error_code& eror, std::size_t){
            delete tmp;
          });
          if (recieved.type() == Type::Bootstrap) {
            request->set_type(Type::Add);
            Address addr(from_router.address(), from_router.port());
            Package package(router, router->connectTo(unRouter(addr)), request);
            this->add_routers(from_router, request);
            router->send(package, [&](Router*, bool){});
          } else if (recieved.type() == Type::Add) {
              for (auto it = recieved.connections().begin(); it != recieved.connections().end(); ++it) {
                std::cout << "Add Router: " << it->address() << ":" << it->port() << std::endl;
                request->set_type(Type::Bootstrap);
                request->set_version(router->version);
                auto add_router = std::make_shared<unRouter>(Address(it->address(), it->port()));
                if (router->inConnections(add_router)) {
                  return;
                }
                Package package(router,
                    router->connectTo(add_router),
                    request
                );
                router->send(package, [add_router = std::move(add_router)](Router* router, bool timeout) {
                  if (!timeout) {
                    std::cout << "Succesful Connection" << std::endl;
                    return;
                  }
                  std::cout << *add_router->getAddress() << ":" << *add_router->getPort() << std::endl;
                  std::cout << "Timeout!" << std::endl;
                  if (router->inConnections(add_router)) {
                    router->disConnect(add_router);
                  }
                  return;
                });
            }
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
