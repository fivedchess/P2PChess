#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess{
  class unRouter : public Connectable{
    public:
      Address address;
      const std::string* getAddress() const override;
      const short* getPort() const override;
      unRouter(Address address);
      bool operator==(const unRouter& another) const;
  };
};
