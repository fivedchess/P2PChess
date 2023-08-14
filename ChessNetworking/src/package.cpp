#include <ChessNetworking/package.h>
#include <ChessNetworking/connectable.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess {
  Package::Package(Connectable* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<Request> data) : from(from), to(boost::move(to)), data(data) {}
};
