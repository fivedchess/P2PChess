#include <ChessNetworking/proxy_server.h>

namespace Chess {
  ProxyServer::ProxyServer(Address address) : address(address), endpoint(boost::asio::ip::address::from_string(address.ip), address.port){}
  short* ProxyServer::getPort(){
    return &this->address.port;
  }
  std::string* ProxyServer::getAddress(){
    return &this->address.ip;
  }
};
