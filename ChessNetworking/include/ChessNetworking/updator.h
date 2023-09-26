#include <ChessNetworking/router.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
#pragma once
namespace Chess {
  class Updator : public std::enable_shared_from_this<Updator> {
    protected:
      boost::asio::io_context& io_context;
      Router& router;
    public:
      void run();
      Updator(boost::asio::io_context& io_context, Router& router);
  };
};
