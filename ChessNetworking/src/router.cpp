#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/unrouter.h>
namespace Chess{
  void Router::do_accept(){
    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
          do_accept();
        });
  }
  Router::Router(boost::asio::io_context& io_context, Address address) : address(address), server_endpoint(boost::asio::ip::tcp::v6(), address.port), acceptor(io_context, server_endpoint), io_context(io_context) {}
  void Router::run(){
    do_accept();
    boost::shared_ptr<std::map<std::string, std::string>> Data = boost::make_shared<std::map<std::string, std::string>>();
    (*Data)["Type"] = "Bootstrap";
    ProxyServer proxy(Address("127.0.0.1:4447"));
    Package package(this, new unRouter(Address("ivt5wiimhwpo56td6eodn2n3fduug3bglqvqewbk2jnyl4hcimea.b32.i2p:6002")), Data);
    boost::make_shared<Socks5Connection>(std::move(boost::asio::ip::tcp::socket(io_context)), proxy, package)->run();
    //io_context.run();
  }
  std::string* Router::getAddress() {
    return &this->address.ip;
  }
  short* Router::getPort() {
    return &this->address.port;
  }
};

