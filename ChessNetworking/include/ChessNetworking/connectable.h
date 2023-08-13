#pragma once
namespace Chess {
  class Address;
  struct Connectable {
    virtual const std::string* getAddress() const = 0;
    virtual const short* getPort() const = 0;
  };
};
