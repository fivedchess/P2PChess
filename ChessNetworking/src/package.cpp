#include <ChessNetworking/package.h>
#include <ChessNetworking/connectable.h>
namespace Chess {
  Package::Package(Connectable* from, Connectable* to, boost::shared_ptr<std::map<std::string, std::string>> data) : from(from), to(to), data(data) {}
  Package::Package(Connectable* from, std::unordered_set<unRouter, unRouter_hash>::iterator to, boost::shared_ptr<std::map<std::string, std::string>> data) : from(from), to(const_cast<unRouter*>(&(*to))), data(data) {}
};
