#pragma once
namespace Chess {
  class Package;
  class Connection : public boost::enable_shared_from_this<Connection> {
      protected:
        boost::asio::ip::tcp::socket socket;
        std::stringstream msgpack;
        Package& package;
      public:
        void run();
        Connection(boost::asio::ip::tcp::socket socket, Package& package);
    };
};
