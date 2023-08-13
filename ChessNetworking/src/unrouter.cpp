#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/address.h>
namespace Chess {
  const std::string* unRouter::getAddress() const{
    return &this->address.ip;
  }
  const short* unRouter::getPort() const{
    return &this->address.port;
  }
  unRouter::unRouter(Address address) : address(address){}
  bool unRouter::operator==(const unRouter& another) const{
    return (this->address.ip == another.address.ip && this->address.port == another.address.port);
  }
};
