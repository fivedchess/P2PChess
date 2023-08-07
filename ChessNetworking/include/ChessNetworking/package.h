#pragma once
#include <ChessNetworking/address.h>
namespace Chess {
  class Address;
  class Router;
  class unRouter{
    public:
      Address address;
      unRouter(Address address) : address(address){}
    };
  class Package{
    public:
      unRouter* from;
      unRouter* to;
      boost::shared_ptr<std::map<std::string, std::string>> data;
      Package(unRouter* from, unRouter* to, boost::shared_ptr<std::map<std::string, std::string>> data) : from(from), to(to), data(data) {}
  };
}
