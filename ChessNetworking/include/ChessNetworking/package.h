#pragma once
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/unrouter_hash.h>
namespace Chess {
  class Connectable;
  class Package{
    public:
      Connectable* from;
      Connectable* to;
      boost::shared_ptr<std::map<std::string, std::string>> data;
      Package(Connectable* from, Connectable* to, boost::shared_ptr<std::map<std::string, std::string>> data);
      Package(Connectable* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<std::map<std::string, std::string>> data);
      //Package(Connectable* from, const Connectable* to, boost::shared_ptr<std::map<std::string, std::string>> data);
  };
}
