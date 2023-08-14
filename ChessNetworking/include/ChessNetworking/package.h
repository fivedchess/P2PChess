#pragma once
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  class Connectable;
  class Package{
    public:
      Connectable* from;
      std::unordered_set<unRouter, unRouter_hash>::iterator to;
      boost::shared_ptr<Request> data;
      Package(Connectable* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<Request> data);
  };
}
