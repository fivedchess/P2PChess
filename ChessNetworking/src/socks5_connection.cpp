#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/package.h>
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
      }
    });
  }
  void Socks5Connection::handshake(){
    auto self(shared_from_this());
    boost::asio::async_write(this->socket, boost::asio::buffer(this->handshake_request), [this, self](const boost::system::error_code& error, std::size_t){
      if(!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->handshake_reply), [this, self](const boost::system::error_code& error, std::size_t){
          if (!error){
            if(this->handshake_reply[0] == 0x05 && this->handshake_reply[1] == 0x00){
              this->connect();
            } else {
              std::cout << error.message() << std::endl;
            }
          } else {
            std::cout << error.message() << std::endl;
          }
        });
      }
    });
  }
  void Socks5Connection::connect() {
    auto self(shared_from_this());
    this->connect_request.insert(this->connect_request.end(), router->getAddress()->begin(), router->getAddress()->end());
    this->connect_request.push_back(static_cast<uint8_t>((*this->router->getPort()) & 0xFF));
    this->connect_request.push_back(static_cast<uint8_t>(((*this->router->getPort()) >> 8) & 0xFF));
    boost::asio::async_write(this->socket, boost::asio::buffer(this->connect_request), [this, self](const boost::system::error_code& error, std::size_t){
      if (!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->connect_reply), [this, self](const boost::system::error_code& error, std::size_t){
          if (this->connect_reply[1] == 0x00) {
            this->connect_handler();
          } else {
            std::cout << "connect_reply[1] != 0x00" << std::endl;
          }
        });
      } else {
        std::cout << error.message() << std::endl;
      }
    });
  }
  void Socks5Connection::connect_handler() {
    auto self(shared_from_this());
    (*this->data)["From"] = *this->from->getAddress() + ":" + std::to_string(*this->from->getPort());
    (*this->data)["To"] = *this->router->getAddress() + ":" + std::to_string(*this->router->getPort());
    msgpack::pack(msgpack, (*this->data));
    boost::asio::async_write(this->socket, boost::asio::buffer(msgpack.str()), [this, self](const boost::system::error_code& error, std::size_t){});
  }
  void Socks5Connection::run() {
    this->proxy_connect();
  }
};
