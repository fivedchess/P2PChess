#include <ChessNetworking/router.h>
#include <ChessNetworking/address.h>
#include <iostream>
#include <string>
int main(){
  Chess::Address address("127.0.0.1:1312");
  Chess::Router router(address);
  router.run();
}
