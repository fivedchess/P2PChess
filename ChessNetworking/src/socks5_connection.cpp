#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/router.h>
namespace Chess {
  Socks5Connection::Socks5Connection(boost::asio::ip::tcp::socket socket, ProxyServer& proxy, Package& package) : socket(std::move(socket)), proxy(proxy), router(package.to), data(package.data), from(package.from) {
  }
  void Socks5Connection::proxy_connect() {
    auto self(shared_from_this());
    this->socket.async_connect(this->proxy.endpoint, [this, self](const boost::system::error_code& error){
      if (!error) {
        this->handshake();
      } else {
        std::cout << error.message() << std::endl;
        this->socket.close();
      }
    });
  }
  void Socks5Connection::handshake(){
    auto self(shared_from_this());
    boost::asio::async_write(this->socket, boost::asio::buffer(this->handshake_request), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
      if(!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->handshake_reply), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
          if (!error){
            if(this->handshake_reply[0] == 0x05 && this->handshake_reply[1] == 0x00){
              this->connect();
            } else {
              std::cout << error.message() << std::endl;
              this->socket.close();
            }
          } else {
            std::cout << error.message() << std::endl;
            this->socket.close();
          }
        });
      }
    });
  }
  void Socks5Connection::connect() {
    auto self(shared_from_this());
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v6(), *this->router->getPort());
    uint16_t networkPort = endpoint.port();
    this->connect_request.push_back(router->getAddress()->length());
    this->connect_request.insert(this->connect_request.end(), router->getAddress()->begin(), router->getAddress()->end());
    this->connect_request.push_back(static_cast<uint8_t>(networkPort >> 8) & 0xFF);
    this->connect_request.push_back(static_cast<uint8_t>(networkPort & 0xFF));
    boost::asio::async_write(this->socket, boost::asio::buffer(this->connect_request), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
      if (!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->connect_reply), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
          if (this->connect_reply[1] == 0x00) {
            this->connect_handler();
          } else {
            std::cout << "connect_reply[1] != 0x00" << std::endl;
            this->socket.close();
          }
        });
      } else {
        std::cout << error.message() << std::endl;
        this->socket.close();
      }
    });
  }
  void Socks5Connection::connect_handler() {
    auto self(shared_from_this());
    SerialisedConnectable* from_router = new SerialisedConnectable;
    from_router->set_address(*this->from->getAddress());
    from_router->set_port(*this->from->getPort());
    SerialisedConnectable* to_router = new SerialisedConnectable;
    to_router->set_address(*this->router->getAddress());
    to_router->set_port(*this->router->getPort());
    this->data->set_allocated_from(from_router);
    this->data->set_allocated_to(to_router);
    boost::asio::async_write(this->socket, boost::asio::buffer(this->data->SerializeAsString()), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){

    });
  }
  void Socks5Connection::run() {
    this->proxy_connect();
  }
};
