#include <ChessNetworking/client.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/timer.h>
#include <ChessNetworking/connector.h>
namespace Chess {
  Client::Client(Connector* connector) :
      package(connector->package()),
      socket(connector->socket()),
      timer(connector->timer()) {
    //
  }
  void Client::run(const std::shared_ptr<Connector> connector) {
    SerialisedConnectable* from_router = new SerialisedConnectable;
    from_router->set_address(this->package.from.getAddress());
    from_router->set_port(this->package.from.getPort());
    SerialisedConnectable* to_router = new SerialisedConnectable;
    to_router->set_address(this->package.to.getAddress());
    to_router->set_port(this->package.to.getPort());
    this->package.data.set_allocated_from(from_router);
    this->package.data.set_allocated_to(to_router);
    boost::asio::async_write(this->socket, boost::asio::buffer(this->package.data.SerializeAsString()), boost::asio::transfer_all(), [this, connector](const boost::system::error_code& error, std::size_t){
      if (error) {
        std::cout << error.message() << std::endl;
        this->timer.status = true;
        this->timer.timer.cancel_one();
        return;
      };
      boost::asio::async_read(this->socket, boost::asio::buffer(this->reply), [this, connector](const boost::system::error_code& error, std::size_t size){
        Request reply;
        reply.ParseFromArray(this->reply.data(), size);
        if (reply.version() == this->package.from.version() && reply.type() == Type::Success) {
          this->timer.status = false;
          this->timer.timer.cancel_one();
          return;
        }
        this->timer.status = true;
        this->timer.timer.cancel_one();
      });
    });
  };
};
