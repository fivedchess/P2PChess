#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/address.h>
namespace Chess {
  std::string* unRouter::getAddress(){
    return &this->address.ip;
  }
  short* unRouter::getPort(){
    return &this->address.port;
  }
  unRouter::unRouter(Address address) : address(address){}
  bool unRouter::operator==(const unRouter& another) const{
    return (this->address.ip == another.address.ip && this->address.port == another.address.port);
  }
};
