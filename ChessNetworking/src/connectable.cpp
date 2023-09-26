#include <ChessNetworking/connectable.h>
namespace Chess {
  const int Connectable::version() const {return 0;};
  const bool Connectable::operator==(const Connectable& n) const {
    return (this->getPort() == n.getPort() && this->getAddress() == n.getAddress());
  };
  const bool Connectable::operator!=(const Connectable& n) const {
    return !(*this == n);
  };
};
