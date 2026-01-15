

#include <queue>
#include <string>
#include <type_traits>
#include <vector>
// base class for any resource
class Resource {
   public:
    Resource(std::string name, uint32_t rid) : _name(name), _rid(rid) {}

    const std::string &getName() const { return _name; }
    uint32_t getRID() const { return _rid; }

   private:
    std::string _name;
    uint32_t _rid;
};

// identifies a resource
class ResourceHandle {
    uint32_t _rid;
    size_t _index;

   public:
    ResourceHandle(uint32_t rid, size_t index) : _rid(rid), _index(index) {}
    uint32_t getRID() const { return _rid; }
    uint32_t getIndex() const { return _index; }
};

// Manager class to allocate and get instances of a type of resource
template <typename T, typename TH>
class ResourceManager {
    static_assert(std::is_base_of_v<ResourceHandle, TH>,
                  "The ResourceHandle type must be based of ResourceHandle");
    static_assert(std::is_base_of_v<Resource, T>,
                  "The Resource type must be based of Resource");

   public:
    ResourceManager(size_t initial_size) : _resources(initial_size) {}
    template <typename... Ts>
    TH create(std::string name, Ts... args) {
        size_t index = _resources.size();
        if (not _free_indexes.empty()) {
            _resources[_free_indexes.front()] = T(name, _nextid, args...);
            index = _free_indexes.front();
            _free_indexes.pop();
        } else {
            _resources.push_back(T(name, _nextid, args...));
        }
        auto h = TH(_nextid, index);
        _nextid++;
        return h;
    }
    T &get(const TH &handle) { return _resources[handle.getIndex()]; }
    const std::vector<T> &getAll() { return _resources; }
    void clear() { _resources.clear(); }
    void destroy(const TH &handle) { _free_indexes.push(handle.getIndex()); }

   private:
    std::vector<T> _resources;
    std::queue<size_t> _free_indexes;
    uint32_t _nextid;
};
