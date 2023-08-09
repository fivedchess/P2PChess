#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/package.h>
namespace Chess {
  Socks5Connection::Socks5Connection(boost::asio::io_context& io_context, ProxyServer& proxy, Package& package) : socket(boost::asio::ip::tcp::socket(io_context)), proxy(proxy), router(package.to), data(package.data), from(package.from) {
  }
  void Socks5Connection::proxy_connect() {
    this->socket.async_connect(this->proxy.endpoint, [&](const boost::system::error_code& error){
      if (!error) {
        this->handshake();
      }
    });
  }
  void Socks5Connection::handshake(){
    boost::asio::async_write(this->socket, boost::asio::buffer(this->handshake_request), [&](const boost::system::error_code& error, std::size_t){
      if(!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->handshake_reply), [&](const boost::system::error_code& error, std::size_t){
          if (!error){
            if(this->handshake_reply[0] == 0x05 && this->handshake_reply[1] == 0x00){
              this->connect();
            }
          }
        });
      }
    });
  }
  void Socks5Connection::connect() {
    this->connect_request.insert(this->connect_request.end(), router->getAddress()->begin(), router->getAddress()->end());
    this->connect_request.push_back(static_cast<uint8_t>((*this->router->getPort()) & 0xFF));
    this->connect_request.push_back(static_cast<uint8_t>(((*this->router->getPort()) >> 8) & 0xFF));
    boost::asio::async_write(this->socket, boost::asio::buffer(this->connect_request), [&](const boost::system::error_code& error, std::size_t){
      if (!error) {
        boost::asio::async_read(this->socket, boost::asio::buffer(this->connect_reply), [&](const boost::system::error_code& error, std::size_t){
          if (this->connect_reply[1] == 0x00) {
            this->connect_handler();
          }
        });
      }
    });
  }
  void Socks5Connection::connect_handler() {
    (*this->data)["From"] = *this->from->getAddress() + ":" + std::to_string(*this->from->getPort());
    (*this->data)["To"] = *this->router->getAddress() + ":" + std::to_string(*this->router->getPort());
    msgpack::pack(msgpack, (*this->data));
    boost::asio::async_write(this->socket, boost::asio::buffer(msgpack.str()), [&](const boost::system::error_code& error, std::size_t){});
  }
  void Socks5Connection::run() {
    this->proxy_connect();
  }
};
