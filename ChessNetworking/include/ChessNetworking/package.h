#pragma once
namespace Chess {
  class Connectable;
  class Package{
    public:
      Connectable* from;
      Connectable* to;
      boost::shared_ptr<std::map<std::string, std::string>> data;
      Package(Connectable* from, Connectable* to, boost::shared_ptr<std::map<std::string, std::string>> data);
  };
}
