#include <ChessNetworking/address.h>
namespace Chess {
  Address::Address(const char* address) :
          ip(std::string(address).substr(0, std::string(address).find(":"))),
          port(std::stoi(std::string(address).substr(std::string(address).find(":") + 1))) {}
  std::string Address::getString() {
        return ip + ":" + std::to_string(port);
      }
  Address::Address(std::string address) :
          ip(address.substr(0, address.find(":"))),
          port(std::stoi(address.substr(address.find(":") + 1))) {}
  Address::Address(std::string ip, short port) :
          ip(ip),
          port(port) {}
};
