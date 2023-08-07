#pragma once
namespace Chess {
  class Address;
  class Router;
  class unRouter;
  class Package{
    public:
      unRouter* from;
      unRouter* to;
      boost::shared_ptr<std::map<std::string, std::string>> data;
      Package(unRouter* from, unRouter* to, boost::shared_ptr<std::map<std::string, std::string>> data);
  };
}
