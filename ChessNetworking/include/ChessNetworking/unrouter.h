#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess{
  class unRouter : public Connectable{
    protected:
      Address address;
    public:
      std::string* getAddress() override;
      short* getPort() override;
      unRouter(Address address);
  };
};
