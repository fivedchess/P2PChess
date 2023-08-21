#pragma once
#include <ChessNetworking/router.h>
class Request;
class SerialisedConnectable;
namespace Chess {
  class ServerConnection : public boost::enable_shared_from_this<ServerConnection>{
    protected:
      boost::asio::ip::tcp::socket socket;
      std::vector<uint8_t> data;
      void do_read();
      Router* router;
      void add_routers(SerialisedConnectable& from_router, boost::shared_ptr<Request> request);
    public:
      ServerConnection(boost::asio::ip::tcp::socket, Router*);
      void run();
  };
};
