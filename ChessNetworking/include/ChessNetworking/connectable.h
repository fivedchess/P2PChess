#pragma once
namespace Chess {
  class Address;
  struct Connectabble {
    virtual std::string* getAddress() = 0;
    virtual short* getPort() = 0;
  };
};
