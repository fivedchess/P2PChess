#pragma once
namespace Chess {
  class Address;
  struct Connectable {
    virtual const std::string& getAddress() const = 0;
    virtual const short& getPort() const = 0;
    virtual const int version() const;
    virtual const bool operator==(const Connectable& n) const;
    virtual const bool operator!=(const Connectable& n) const;
  };
};
