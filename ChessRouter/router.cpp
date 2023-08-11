#include <ChessNetworking/router.h>
#include <ChessNetworking/address.h>
#include <iostream>
#include <string>
int main(){
  boost::asio::io_context io_context;
  Chess::Address address("ivt5wiimhwpo56td6eodn2n3fduug3bglqvqewbk2jnyl4hcimea.b32.i2p:1312");
  Chess::Router router(io_context, address);
  router.run();
  io_context.run();
}
