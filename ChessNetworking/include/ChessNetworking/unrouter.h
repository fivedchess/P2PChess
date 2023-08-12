#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess{
  class unRouter : public Connectable{
    public:
      Address address;
      std::string* getAddress() override;
      short* getPort() override;
      unRouter(Address address);
      bool operator==(const unRouter& another) const;
  };
};
