#pragma once
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
#include <ChessSerialisation/request.pb.h>
#include <ChessNetworking/router.h>
namespace Chess {
  class Connectable;
  class Package{
    public:
      Router* from;
      std::unordered_set<unRouter, unRouter_hash>::iterator to;
      boost::shared_ptr<Request> data;
      Package(Router* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<Request> data);
  };
}
