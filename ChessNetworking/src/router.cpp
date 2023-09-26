#include <ChessNetworking/router.h>
#include <ChessNetworking/package.h>
#include <ChessNetworking/socks5_connection.h>
#include <ChessNetworking/unrouter.h>
#include <ChessNetworking/timer.h>
#include <ChessSerialisation/request.pb.h>
namespace Chess{
  class ServerSession {
    protected:
      std::byte data[8192];
      boost::asio::ip::tcp::socket socket;
    public:
      friend class Router;
      ServerSession(boost::asio::ip::tcp::socket socket) : socket(std::move(socket)) {};
  };
  void Router::do_accept(){
    acceptor.async_accept(
        [this](boost::system::error_code error, boost::asio::ip::tcp::socket socket) {
          do_accept();
          if (error) {
            std::cout << error.message() << std::endl;
            return;
          };
          this->connectionAccept(std::make_shared<ServerSession>(std::move(socket)));
        });
  };
  void Router::addRouters(Request& request) {
    this->forConnections([this, &request](const auto& node){
      if(node.getAddress() == this->getAddress() && node.getPort() == this->getPort()) {
        return;
      };
      SerialisedConnectable* myNode = request.add_connections();
      myNode->set_address(node.getAddress());
      myNode->set_port(node.getPort());
      std::cout << "Router added to request: " << myNode->address() << ":" << myNode->port() << std::endl;
    });
  };
  void Router::bootstrapConnection(Request request, SerialisedConnectable from) {
    request.set_type(Type::Add);
    this->addRouters(request);
    Address address(from.address(), from.port());
    Package package(*this, this->connectTo(unRouter(std::move(address))), std::move(request));
    this->send(std::move(package), [this](bool){});
  };
  void Router::addConnection(Request request, Request recieved) {
    for(const auto& element : recieved.connections()) {
      unRouter addRouter(Address(element.address(), element.port()));
      if(this->inConnections(addRouter)) {
        continue;
      };
      std::cout << "Add Router: " << element.address() << ":" << element.port() << std::endl;
      request.set_type(Type::Bootstrap);
      const auto& add_router_ref(this->connectTo(std::move(addRouter)));
      Package package(*this, add_router_ref, std::move(request));
      this->send(std::move(package), [this, &add_router_ref](bool error){
        if(!error) {
          std::cout << "Succesful Connection" << std::endl;
          return;
        };
        std::cout << add_router_ref.getAddress() << ":" << add_router_ref.getPort() << std::endl;
        std::cout << "Error" << std::endl;
        if(this->inConnections(add_router_ref)) {
          this->disConnect(add_router_ref);
        };
        return;
      });
    };
  };
  void Router::connectionAccept(std::shared_ptr<ServerSession> session) {
    session->socket.async_read_some(boost::asio::buffer(session->data), [this, session](const boost::system::error_code& error, std::size_t size){
      if(error) {
        return;
      };
      Request recieved;
      recieved.ParseFromArray(session->data, size);
      Request status;
      status.set_version(this->version());
      status.set_type(Type::Success);
      bool myError = false;
      if(recieved.version() != this->version()) {
        std::cout << "Bad Version" << std::endl;
        myError = true;
      };
      const SerialisedConnectable& from(recieved.from());
      std::cout << "From: " << from.address() << ":" << from.port() << std::endl;
      session->socket.async_write_some(boost::asio::buffer(status.SerializeAsString()),
          [session](const boost::system::error_code& error, std::size_t){
        session->socket.close();
      });
      if(myError) {
        return;
      };
      Request request;
      request.set_version(this->version());
      if(recieved.type() == Type::Bootstrap) {
        return this->bootstrapConnection(std::move(request), from);
      } else if(recieved.type() == Type::Add) {
        return this->addConnection(std::move(request), std::move(recieved));
      };
    });
  };
  void Router::send(Package package, std::function<void(bool)> callback) {
    try {
      boost::asio::steady_timer timer(this->io_context);
      auto container = TimerContainer(std::move(timer), std::move(callback), true);
      boost::asio::ip::tcp::resolver resolver(io_context);
      std::make_shared<Socks5Connection>(boost::asio::ip::tcp::socket(io_context), this->proxy, std::move(package), std::move(container), std::move(resolver))->run();
    } catch (std::exception) {throw;}
  };
  const unRouter& Router::connectTo(unRouter router) {
    return *this->connections.emplace(std::move(router)).first;
  }
  void Router::forConnections(std::function<void(const unRouter&)> callback) {
    for(const auto& element : this->connections) {
      callback(element);
    };
  };
  void Router::disConnect(const unRouter& router) {
    this->connections.erase(router);
  };
  bool Router::inConnections(const unRouter& router) {
    if (this->connections.find(router) == this->connections.end()) {
      return false;
    };
    return true;
  };
  void Router::reseed_connection() {
    Request Data;
    Data.set_type(Type::Bootstrap);
    Data.set_version(this->version_);
    Package package(*this, *this->reseed, std::move(Data));
    this->send(std::move(package), [this](bool error){
      if(!error) {
        return;
      };
      std::cout << "Connection To Reseed Failed" << std::endl;
      this->reseed_connection();
    });
  };
  const void Router::update() {
    std::cout << "UPDATE!" << std::endl;
    this->forConnections([this](const auto& element){
      std::cout << "Updating: "
        << element.getAddress()
        << ":" << element.getPort()
      << std::endl;
      Request request;
      request.set_type(Type::Bootstrap);
      request.set_version(this->version());
      Package package(*this, element, std::move(request));
      this->send(std::move(package), [this, &element](bool timeout){
        if (!this->inConnections(element)) {
          std::cout << "WARN: " << "Already Deleted" << std::endl;
          return;
        };
        if (!timeout) {
          std::cout
          << "Success: "
          << element.getAddress()
          << ":" << element.getPort()
          << std::endl;
          return;
        };
        std::cout << "Failed: "
        << element.getAddress()
        << ":" << element.getPort()
        << std::endl;
        this->disConnect(element);
        return;
      });
    });
    this->updator_timer.expires_after(std::chrono::minutes(5));
    this->updator_timer.async_wait([this](const boost::system::error_code& error){
      this->update();
    });
  };
  Router::Router(boost::asio::io_context& io_context, int argc, const char** argv) :
      address("127.0.0.1:1984"),
      server_endpoint(),
      acceptor(io_context, server_endpoint),
      io_context(io_context),
      version_(10000),
      updator_timer(io_context),
      proxy(Address("127.0.0.1:4447")) {
    this->m_desc.add_options()
    ("help,h", "Produce help message")
    ("address,a",  boost::program_options::value<std::string>()->composing(), "Set the address")
    ("proxy",  boost::program_options::value<std::string>()->default_value("127.0.0.1:4447"), "Set the Proxy")
    ("reseed",  boost::program_options::value<std::string>()->default_value("knvnfuid67t6l7sgh42rrcscjoypxmqcosdc3dxbrlacks4ryaua.b32.i2p:6002"), "Set the Reseed");
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_desc), m_vm);
    boost::program_options::notify(m_vm);
  };
  void Router::run(){
    if (this->m_vm.count("help")){
      std::cout << this->m_desc << std::endl;
      return;
    };
    if (this->m_vm.count("address")){
      this->address = Address(this->m_vm["address"].as<std::string>());
      std::cout << "B32: " << this->address.ip << std::endl;
      std::cout << "Port: " << this->address.port << std::endl;
      this->server_endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), this->address.port);
      this->acceptor = boost::asio::ip::tcp::acceptor(this->io_context, this->server_endpoint);
    } else {
      std::cout << this->m_desc << std::endl;
      return;
    };
    if (this->m_vm.count("proxy")){
      this->proxy = ProxyServer(Address(this->m_vm["proxy"].as<std::string>()));
      std::cout << "Proxy: " << this->proxy.getAddress() << ":" << this->proxy.getPort() << std::endl;
    };
    if (this->m_vm.count("reseed")){
      this->reseed = &this->connectTo(unRouter(Address(this->m_vm["reseed"].as<std::string>())));
      std::cout << "Reseed: " << this->reseed->getAddress() << ":" << this->reseed->getPort() << std::endl;
    };
    this->do_accept();
    this->reseed_connection();
    auto timer = std::make_shared<boost::asio::steady_timer>(io_context, std::chrono::minutes(5));
    timer->async_wait([this, timer](const boost::system::error_code&){
      this->update();
    });
  };
  const std::string& Router::getAddress() const {
    return this->address.ip;
  };
  const short& Router::getPort() const {
    return this->address.port;
  };
  const int Router::version() const {
    return this->version_;
  };
};

