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

};
