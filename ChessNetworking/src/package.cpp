#include <ChessNetworking/package.h>
#include <ChessNetworking/unrouter.h>
namespace Chess {
  Package::Package(unRouter* from, unRouter* to, boost::shared_ptr<std::map<std::string, std::string>> data) : from(from), to(to), data(data) {}
};
