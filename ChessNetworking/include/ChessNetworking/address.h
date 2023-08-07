#pragma once
namespace Chess {
  class Address {
    public:
      std::string ip;
      short port;
      Address(std::string ip, short port);
      Address(std::string address);
      Address(const char* address);
      std::string getString();
  };
};
