#pragma once
#include <ChessNetworking/router.h>
namespace Chess {
  class ServerConnection : public boost::enable_shared_from_this<ServerConnection>{
    protected:
      boost::asio::ip::tcp::socket socket;
      uint8_t data[8128];
      void do_read();
      Router* router;
    public:
      ServerConnection(boost::asio::ip::tcp::socket, Router*);
      void run();
  };
};
