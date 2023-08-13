#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/server_connection.h>
#include <ChessNetworking/unrouter.h>
namespace Chess{
  void Router::do_accept(){
    acceptor.async_accept(
        [this](boost::system::error_code error, boost::asio::ip::tcp::socket socket) {
          if (!error) {
            boost::make_shared<ServerConnection>(boost::move(socket), this)->run();
          }
          do_accept();
        });
  }
  void Router::send(Package& package) {
    boost::make_shared<Socks5Connection>(boost::move(boost::asio::ip::tcp::socket(io_context)), this->proxy, package)->run();
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::connectTo(unRouter router) {
    return this->connections.emplace(boost::move(router)).first;
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::find(unRouter* router) {
    return this->connections.find(*router);
  }
  Router::Router(boost::asio::io_context& io_context, int argc, const char** argv) :
    address("127.0.0.1:1984"),
    server_endpoint(),
    acceptor(io_context, server_endpoint),
    io_context(io_context),
    proxy(Address("127.0.0.1:4447"))
    //reseed(Address("127.0.0.1:1984"))
  {
      this->m_desc.add_options()
      ("help,h", "Produce help message")
      ("address,a",  boost::program_options::value<std::string>()->composing(), "Set the b32")
      ("port,p",  boost::program_options::value<short>()->default_value(1984), "Set the port")
      ("proxy",  boost::program_options::value<std::string>()->default_value("127.0.0.1:4447"), "Set the Proxy")
      ("reseed",  boost::program_options::value<std::string>()->default_value("knvnfuid67t6l7sgh42rrcscjoypxmqcosdc3dxbrlacks4ryaua.b32.i2p:6002"), "Set the Reseed");
      boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_desc), m_vm);
      boost::program_options::notify(m_vm);
    }
  const std::string Router::version = "0.001";
  void Router::run(){
    if (m_vm.count("help")){
      std::cout << this->m_desc << std::endl;
      return;
    }
    if (m_vm.count("address")){
      this->address.ip = m_vm["address"].as<std::string>();
      std::cout << "B32: " << this->address.ip << std::endl;
    } else {
      std::cout << "Please, use --help option for information" << std::endl;
      return;
    }
    if (m_vm.count("proxy")){
      this->proxy = ProxyServer(Address(m_vm["proxy"].as<std::string>()));
      std::cout << "Proxy: " << *this->proxy.getAddress() << ":" << *this->proxy.getPort() << std::endl;
    }
    if (m_vm.count("reseed")){
      reseed = this->connectTo(unRouter(Address(m_vm["reseed"].as<std::string>())));
      std::cout << "Reseed: " << *this->reseed->getAddress() << ":" << *this->reseed->getPort() << std::endl;
    }
    if (m_vm.count("port")){
      this->address.port = m_vm["port"].as<short>();
      this->server_endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), this->address.port);
      this->acceptor = boost::asio::ip::tcp::acceptor(this->io_context, this->server_endpoint);
      std::cout << "Port: " << this->address.port << std::endl;
    }


    do_accept();
    boost::shared_ptr<std::map<std::string, std::string>> Data = boost::make_shared<std::map<std::string, std::string>>();
    (*Data)["Type"] = "Bootstrap";
    Package package(this, this->reseed, Data);
    boost::make_shared<Socks5Connection>(boost::move(boost::asio::ip::tcp::socket(io_context)), proxy, package)->run();
  }
  const std::string* Router::getAddress() const {
    return &this->address.ip;
  }
  const short* Router::getPort() const {
    return &this->address.port;
  }
};

