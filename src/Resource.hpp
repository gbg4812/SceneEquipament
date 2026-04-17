#pragma once
#include <cassert>
#include <cstdint>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

namespace gbg {

enum ResourceFlags {
    DIRTY = 0x01,
    INVISIBLE = 0x02,
};

// base class for any resource
class Resource {
   public:
    // TODO: rid 0 vol dir que és null
    Resource() : _rid(0) {};
    Resource(std::string name, uint32_t rid) : _name(name), _rid(rid) {}

    Resource(const Resource& other) = delete;
    Resource& operator=(const Resource& other) = delete;
    Resource(Resource&& other) = default;
    Resource& operator=(Resource&& other) = default;

    const std::string& getName() const { return _name; }
    uint32_t getRID() const { return _rid; }

    void setFlags(ResourceFlags flags) { _flags = _flags | flags; }

    void unsetFlag(ResourceFlags flags) {
        _flags = _flags & (~flags);  // 1010 0010 -> 1101 & 1010
    }

   private:
    std::string _name;
    uint32_t _rid;
    uint32_t _flags;
};

// identifies a resource
class ResourceHandle {
    uint32_t _rid = 0;
    size_t _index = 0;

   public:
    ResourceHandle(uint32_t rid, size_t index) : _rid(rid), _index(index) {}
    ResourceHandle() : _rid(0), _index(0) {};
    uint32_t getRID() const { return _rid; }
    uint32_t getIndex() const { return _index; }
    bool empty() { return _rid == 0; }

    bool operator==(const ResourceHandle& other) const {
        return (other._index == _index) and (other._rid == _rid);
    }

    explicit operator bool() { return _rid != 0; }
};

// Manager class to allocate and get instances of a type of resource
template <typename T, typename TH>
class ResourceManager {
    static_assert(std::is_base_of_v<ResourceHandle, TH>,
                  "The ResourceHandle type must be based of ResourceHandle");
    static_assert(std::is_base_of_v<Resource, T>,
                  "The Resource type must be based of Resource");
    static_assert(std::is_constructible_v<T, std::string, uint32_t>,
                  "The Resource must have this constructor");
    static_assert(
        std::is_default_constructible_v<T>,
        "The Resource must be default constructible with a call to Resource()");

   public:
    ResourceManager(size_t initial_size = 0) {
        _resources.reserve(initial_size + 1);
        _resources.push_back(T());
    }

    ResourceManager(const ResourceManager& other) = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;
    ResourceManager(ResourceManager&& other) = default;
    ResourceManager& operator=(ResourceManager&& other) = default;

    TH create(std::string name) {
        size_t index = _resources.size();
        if (not _free_indexes.empty()) {
            _resources[_free_indexes.front()] = T(name, _nextid);
            index = _free_indexes.front();
            _free_indexes.pop_front();
        } else {
            _resources.push_back(T(name, _nextid));
        }
        auto h = TH(_nextid, index);
        _nextid++;
        return h;
    }

    T& get(const TH& handle) {
        assert(handle.getRID() == _resources[handle.getIndex()].getRID());
        return _resources[handle.getIndex()];
    }

    T& getRelated(const ResourceHandle& handle) {
        assert(handle.getRID() == _resources[handle.getIndex()].getRID());
        return _resources[handle.getIndex()];
    }
    std::vector<T>& getAll() { return _resources; }
    void clear() {
        _resources.clear();
        while (!_free_indexes.empty()) _free_indexes.pop_front();
    }
    void destroy(const TH& handle) {
        _resources[handle.getIndex()] = T();
        _free_indexes.push_front(handle.getIndex());
    }

    class iterator {
       public:
        iterator(ResourceManager<T, TH>& manager, TH handle)
            : _handl(handle), _manager(manager) {}

        iterator operator++(int) {
            iterator aux = iterator(_manager, _handl);
            ++(*this);
            return aux;
        };

        iterator& operator++() {
            size_t index = _handl.getIndex() + 1;
            while (index < _manager._resources.size() &&
                   _manager._resources[index].getRID() == 0) {
                index++;
            }
            if (index >= _manager._resources.size()) index = 0;
            _handl = TH(_manager._resources[index].getRID(), index);
            return *this;
        };

        bool operator==(const ResourceManager<T, TH>::iterator& other) const {
            return other._handl == this->_handl;
        }

        T& operator*() { return _manager.get(_handl); }

        T& operator->() { return *(*this); }

       private:
        TH _handl;
        ResourceManager<T, TH>& _manager;
    };

    iterator begin() {
        if (_resources.size() > 1)
            return iterator(*this, TH(_resources[1].getRID(), 1));
        return end();
    }

    iterator end() { return iterator(*this, TH(0, 0)); }

   private:
    std::vector<T> _resources;
    std::list<size_t> _free_indexes;
    // 0 is reserved
    uint32_t _nextid = 1;
};

}  // namespace gbg
