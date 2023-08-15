#include <ChessNetworking/package.h>
#include <ChessNetworking/connectable.h>
#include <ChessSerialisation/request.pb.h>
#include <ChessNetworking/router.h>
namespace Chess {
  Package::Package(Router* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<Request> data) : from(from), to(boost::move(to)), data(data) {}
};
