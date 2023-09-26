#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/proxy_server.h>
#include <ChessNetworking/connectable.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/router.h>
#include <ChessNetworking/client.h>
namespace Chess {
  Socks5Connection::Socks5Connection(boost::asio::ip::tcp::socket socket, ProxyServer& proxy, Package package, TimerContainer timer, boost::asio::ip::tcp::resolver resolver) :
      socket_(std::move(socket)),
      proxy(proxy),
      package_(std::move(package)),
      timer_(std::move(timer)),
      client(this),
      resolver(std::move(resolver)) {
  };
  void Socks5Connection::proxy_connect() noexcept {
    auto self(shared_from_this());
    boost::asio::ip::tcp::resolver::query query(this->proxy.getAddress(), std::to_string(this->proxy.getPort()));
    this->resolver.async_resolve(query, [self, this](const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator it){
      if(error) {
        std::cout << error.message() << std::endl;
        this->socket_.close();
        return;
      };
      this->socket_.async_connect(*it, [this, self](const boost::system::error_code& error){
        if (!error) {
          return this->handshake();
        };
        std::cout << error.message() << std::endl;
        this->socket_.close();
      });
    });
  };
  void Socks5Connection::handshake() noexcept {
    auto self(shared_from_this());
    boost::asio::async_write(this->socket_, boost::asio::buffer(this->handshake_request), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
      if(error) {
        std::cout << error.message() << std::endl;
        this->socket_.close();
        return;
      };
      boost::asio::async_read(this->socket_, boost::asio::buffer(this->handshake_reply), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
        if (error){
          std::cout << error.message() << std::endl;
          this->socket_.close();
          return;
        };
        if(this->handshake_reply[0] == std::byte(0x05) && this->handshake_reply[1] == std::byte(0x00)){
          return this->connect();
        };
        std::cout << "Bad Reply" << std::endl;
        this->socket_.close();
      });
    });
  };
  void Socks5Connection::connect() noexcept {
    auto self(shared_from_this());
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v6(), this->package_.to.getPort());
    uint16_t networkPort = endpoint.port();
    auto* it = this->connect_request.begin()+4;
    *it = std::byte(this->package_.to.getAddress().length());
    it++;
    auto size = this->package_.to.getAddress().size();
    auto* addressIt = reinterpret_cast<const std::byte*>(this->package_.to.getAddress().data());
    std::copy(addressIt, addressIt+size, it);
    it+=size;
    *it = std::byte(static_cast<uint8_t>(networkPort >> 8) & 0xFF);
    it++;
    *it = static_cast<std::byte>(networkPort & 0xFF);
    auto* timer = &this->timer_;
    timer->timer.expires_from_now(std::chrono::seconds(60));
    timer->timer.async_wait([timer, self, this](const boost::system::error_code& error) {
      timer->socket->cancel();
      timer->callback(timer->status);
    });
    for(std::size_t i = 0; i<size+7; i++) {
       //std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(this->connect_request[i]) << " ";
    };
    //std::cout << std::endl;
    boost::asio::async_write(this->socket_, boost::asio::buffer(this->connect_request, size+7), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
      if (error) {
        std::cout << error.message() << std::endl;
        this->timer_.status = true;
        this->timer_.timer.cancel_one();
        return;
      };
      boost::asio::async_read(this->socket_, boost::asio::buffer(this->connect_reply), boost::asio::transfer_all(), [this, self](const boost::system::error_code& error, std::size_t){
        if(error) {
          std::cout << error.message() << std::endl;
          this->socket_.close();
          return;
        };
        if (this->connect_reply[1] == std::byte(0x00)) {
          return this->connect_handler();
        };
        std::cout << "connect_reply[1] != 0x00" << std::endl;
        this->timer_.status = true;
        this->timer_.timer.cancel_one();
        return;
      });
    });
  };
  void Socks5Connection::connect_handler() noexcept {
    auto self(shared_from_this());
    this->client.run(self);
  };
  void Socks5Connection::run() noexcept {
    this->timer_.socket = &this->socket_;
    this->proxy_connect();
  };
  Package& Socks5Connection::package() noexcept {
    return this->package_;
  };
  TimerContainer& Socks5Connection::timer() noexcept {
    return this->timer_;
  };
  boost::asio::ip::tcp::socket& Socks5Connection::socket() noexcept{
    return this->socket_;
  };
  Socks5Connection::~Socks5Connection() noexcept = default;
};
