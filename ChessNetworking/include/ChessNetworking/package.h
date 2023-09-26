#pragma once
#include <ChessNetworking/connectable.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  class Package{
    public:
      const Connectable& from;
      const Connectable& to;
      Request data;
      Package(Package&) = delete;
      Package(Package&&);
      Package(const Connectable& from, const Connectable& to, Request data);
  };
}
