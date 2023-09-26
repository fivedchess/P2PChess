#include <ChessNetworking/proxy_server.h>

namespace Chess {
  ProxyServer::ProxyServer(Address address) :
      address(address) {
    //
  };
  const short& ProxyServer::getPort() const{
    return this->address.port;
  };
  const std::string& ProxyServer::getAddress() const{
    return this->address.ip;
  };
};
