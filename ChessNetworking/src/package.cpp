#include <ChessNetworking/package.h>
#include <ChessNetworking/connectable.h>
#include <ChessSerialisation/request.pb.h>
#include <ChessNetworking/router.h>
namespace Chess {
  Package::Package(const Connectable& from, const Connectable& to, Request data) :
      from(from),
      to(to),
      data(std::move(data)) {}
  Package::Package(Package&& package) :
      from(package.from),
      to(package.to),
      data(std::move(package.data)) {};
};
