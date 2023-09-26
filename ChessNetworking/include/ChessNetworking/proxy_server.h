#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
  class ProxyServer : public Connectable {
    protected:
      Address address;
    public:
      const std::string& getAddress() const override;
      const short& getPort() const override;
      ProxyServer(Address address);
  };
};
