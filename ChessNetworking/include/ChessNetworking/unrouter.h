#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess{
  class unRouter : public Connectabble{
    protected:
      Address address;
    public:
      Address* getAddress();
      unRouter(Address address);
  };
};
