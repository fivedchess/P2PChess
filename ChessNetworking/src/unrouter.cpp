#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/address.h>
namespace Chess {
  Address* unRouter::getAddress(){
    return &this->address;
  }
  unRouter::unRouter(Address address) : address(address){}

};
