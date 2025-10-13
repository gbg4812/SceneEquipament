

#include <string>
class Resource {
   public:
    Resource(std::string name, uint32_t rid) : _name(name), _rid(rid) {}
    Resource(Resource &&) = default;
    Resource(const Resource &) = default;
    Resource &operator=(Resource &&) = default;
    Resource &operator=(const Resource &) = default;
    ~Resource() {}

    const std::string &getName() const { return _name; }
    const uint32_t &getRID() const { return _rid; }

   private:
    std::string _name;
    uint32_t _rid;
};
