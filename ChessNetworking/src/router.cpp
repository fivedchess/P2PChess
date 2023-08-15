#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/server_connection.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/updator.h>
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
  void Router::send(Package& package, std::function<void(Router*, bool)> callback) {
    try {
      boost::asio::steady_timer timer(this->io_context);
      boost::make_shared<Socks5Connection>(boost::asio::ip::tcp::socket(io_context), this->proxy, package, std::move(timer), std::move(callback))->run();
    } catch (std::exception) {throw;}
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::connectTo(unRouter router) {
    return this->connections.emplace(std::move(router)).first;
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::connectTo(const std::shared_ptr<unRouter>& router) {
    return this->connections.emplace(*router).first;
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::find(const std::shared_ptr<unRouter>& router) {
    return this->connections.find(*router);
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::find(const unRouter& router) {
    return this->connections.find(router);
  }
  std::unordered_set<unRouter, unRouter_hash>::iterator Router::find(const unRouter* router) {
    return this->connections.find(*router);
  }
  const std::unordered_set<unRouter, unRouter_hash>* Router::getConnections() {
    return &this->connections;
  }
  void Router::disConnect(const std::shared_ptr<unRouter>& router) {
    this->connections.erase(*router);
  }
  void Router::disConnect(const unRouter& router) {
    this->connections.erase(router);
  }
  void Router::disConnect(std::unordered_set<unRouter, unRouter_hash>::iterator router) {
    this->connections.erase(router);
  }
  bool Router::inConnections(const std::shared_ptr<unRouter>& router) {
    if (this->find(router) == this->connections.end()) {
      return false;
    }
    return true;
  }
  bool Router::inConnections(const unRouter* router) {
    if (this->find(router) == this->connections.end()) {
      return false;
    }
    return true;
  }
  bool Router::inConnections(std::unordered_set<unRouter, unRouter_hash>::iterator router) {
    if (router == this->connections.end()) {
      return false;
    }
    return true;
  }

  Router::Router(boost::asio::io_context& io_context, int argc, const char** argv) :
    address("127.0.0.1:1984"),
    server_endpoint(),
    acceptor(io_context, server_endpoint),
    io_context(io_context),
    proxy(Address("127.0.0.1:4447")) {
      this->m_desc.add_options()
      ("help,h", "Produce help message")
      ("address,a",  boost::program_options::value<std::string>()->composing(), "Set the b32")
      ("port,p",  boost::program_options::value<short>()->default_value(1984), "Set the port")
      ("proxy",  boost::program_options::value<std::string>()->default_value("127.0.0.1:4447"), "Set the Proxy")
      ("reseed",  boost::program_options::value<std::string>()->default_value("knvnfuid67t6l7sgh42rrcscjoypxmqcosdc3dxbrlacks4ryaua.b32.i2p:6002"), "Set the Reseed");
      boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_desc), m_vm);
      boost::program_options::notify(m_vm);
    }
  const int Router::version = 10000;
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
    boost::shared_ptr<Request> Data = boost::make_shared<Request>();
    Data->set_type(Type::Bootstrap);
    Data->set_version(Router::version);

    Package package(this, this->reseed, Data);
    this->send(package, [](Router*, bool){});
    auto timer = std::make_shared<boost::asio::steady_timer>(io_context, std::chrono::minutes(5));
    timer->async_wait([this, timer](const boost::system::error_code&){
      std::make_shared<Updator>(this->io_context, this)->run();
    });
  }
  const std::string* Router::getAddress() const {
    return &this->address.ip;
  }
  const short* Router::getPort() const {
    return &this->address.port;
  }
};

