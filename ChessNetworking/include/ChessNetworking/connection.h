#pragma once
namespace Chess {
  class Package;
  class Connection{
      protected:
        boost::asio::io_context* io_context;
        boost::asio::ip::tcp::socket socket;
        std::stringstream msgpack;
      public:
        Connection(boost::asio::io_context* io_context, Package& package);
    };
};
