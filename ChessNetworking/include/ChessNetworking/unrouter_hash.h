#pragma once
#include <ChessNetworking/unrouter.h>
namespace Chess {
  struct unRouter_hash {
    std::size_t operator()(const unRouter& n) const;
  };
};
