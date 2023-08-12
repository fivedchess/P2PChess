#include <ChessNetworking/router.h>
int main(int argc, const char* argv[]){
  boost::asio::io_context io_context;
  Chess::Router router(io_context, argc, argv);
  router.run();
  io_context.run();
}
