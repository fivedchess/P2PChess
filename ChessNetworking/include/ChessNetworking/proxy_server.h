#pragma once
#include <ChessNetworking/address.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
  class ProxyServer : public Connectabble {
    protected:
      Address address;
    public:
      boost::asio::ip::tcp::endpoint endpoint;
      std::string* getAddress() override;
      short* getPort() override;
      ProxyServer(Address address);
  };
};
