#include <ChessNetworking/unrouter_hash.h>
namespace Chess {
    std::size_t unRouter_hash::operator()(const unRouter& n) const{
        return std::hash<std::string>()(n.address.ip) ^ std::hash<int>()(n.address.port);
    };
};
