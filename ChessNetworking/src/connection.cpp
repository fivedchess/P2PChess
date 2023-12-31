#include <ChessNetworking/connection.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/unrouter.h>
namespace Chess {
  Connection::Connection(boost::asio::io_context* io_context, Package& package) : io_context(io_context), socket(boost::asio::ip::tcp::socket(*io_context)){
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(package.to->getAddress()->ip), package.to->getAddress()->port);
    (*package.data)["From"] = package.from->getAddress()->getString();
    (*package.data)["To"] = package.to->getAddress()->getString();
    socket.async_connect(endpoint, [&](boost::system::error_code error) {
      if (!error) {
        boost::asio::async_write(socket, boost::asio::buffer("Change IT!!!"), [&](boost::system::error_code error, std::size_t){
          std::cout << "CONNECTION\n";
          socket.close();
        });
      } else {
        std::cout << error.message() << std::endl;
      }
    });
  }
};
