#pragma once
#include <cassert>
#include <cstdint>
#include <list>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace gbg {

#define BUILD_ID(gen, idx) (gen << 20) | (idx & 0x000FFFFF)
#define GEN(rid) (rid >> 20)
#define INDEX(rid) (rid & 0x000FFFFF)

// identifies a resource
class ResourceHandle {
    uint32_t _rid = 0;

   public:
    ResourceHandle(uint32_t rid) : _rid(rid) {}
    ResourceHandle(uint32_t gen, uint32_t idx) : _rid(BUILD_ID(gen, idx)) {}
    ResourceHandle() : _rid(0) {};
    uint32_t getRID() const { return _rid; }
    uint32_t getIndex() const { return INDEX(_rid); }
    uint32_t getGen() const { return GEN(_rid); }
    bool empty() { return _rid == 0; }

    bool operator==(const ResourceHandle& other) const {
        return (other._rid == _rid);
    }

    explicit operator bool() const { return INDEX(_rid); }
};

// base class for any resource
template <typename TH>
class Resource {
    static_assert(std::is_base_of_v<ResourceHandle, TH>,
                  "TH must be based on ResourceHandle");

   public:
    // TODO: rid 0 vol dir que és null
    Resource() {}
    Resource(uint32_t rid) : _rid(rid) {}
    Resource(std::string name, uint32_t rid) : _name(name), _rid(rid) {}

    Resource(const Resource& other) = delete;
    Resource& operator=(const Resource& other) = delete;
    Resource(Resource&& other) = default;
    Resource& operator=(Resource&& other) = default;

    const std::string& getName() const { return _name; }
    uint32_t getRID() const { return _rid; }
    TH getHandle() const { return _rid; }

   private:
    std::string _name;
    uint32_t _rid = 0;
};

// Manager class to allocate and get instances of a type of resource
template <typename T, typename TH>
class ResourceManager {
    static_assert(std::is_base_of_v<ResourceHandle, TH>,
                  "The TH type must be based of ResourceHandle");
    static_assert(std::is_base_of_v<Resource<TH>, T>,
                  "The T type must be based of Resource");
    static_assert(std::is_constructible_v<T, std::string, uint32_t>,
                  "The T type must have this constructor");
    static_assert(
        std::is_default_constructible_v<T>,
        "The T type must be default constructible with a call to Resource()");

   public:
    ResourceManager(size_t initial_size = 20) {
        _resources.reserve(initial_size + 1);
        _resources.push_back(T());
    }

    ResourceManager(const ResourceManager& other) = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;
    ResourceManager(ResourceManager&& other) = default;
    ResourceManager& operator=(ResourceManager&& other) = default;

    uint32_t nextIndex() const {
        if (not _free_indexes.empty()) {
            return _free_indexes.front();
        } else {
            return _resources.size();
        }
    }

    TH create(const std::string& name) {
        size_t index = _resources.size();
        if (not _free_indexes.empty()) {
            index = _free_indexes.front();
            _resources[index] =
                T(name, BUILD_ID(GEN(_resources[index].getRID()), index));
            _free_indexes.pop_front();
        } else {
            _resources.push_back(T(name, BUILD_ID(0, index)));
        }
        return _resources[index].getHandle();
    }

    TH create(TH handle) {
        if (handle.getIndex() >= _resources.size())
            _resources.resize(handle.getIndex() + 1);
        else
            assert(not INDEX(_resources[handle.getIndex()]
                                 .getRID()));  // enshure we are creating on an
                                               // empty spot
        _resources[handle.getIndex()] = T(handle.getRID());
        return handle;
    }

    T& get(TH handle) {
        assert(handle.getIndex() != 0);
        assert(handle == _resources[handle.getIndex()].getHandle());
        return _resources[handle.getIndex()];
    }

    /*
     * @warning Not eficient, makes a linear search!
     */
    T& getByName(std::string name) {
        for (TH rh : *this) {
            if (get(rh).getName() == name) return get(rh);
        }
        throw std::runtime_error("Resource by name: " + name +
                                 " does not exist!");
    }

    std::vector<T>& getAll() { return _resources; }

    void clear() {
        _resources.clear();
        while (!_free_indexes.empty()) _free_indexes.pop_front();
    }

    void destroy(TH handle) {
        assert(handle.getIndex() != 0);
        assert(handle == _resources[handle.getIndex()].getHandle());
        _resources[handle.getIndex()] = T(BUILD_ID(handle.getGen() + 1, 0));
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
                   not _manager._resources[index].getHandle().getIndex()) {
                index++;
            }
            if (index >= _manager._resources.size()) index = 0;
            _handl = _manager._resources[index].getHandle();
            return *this;
        };

        bool operator==(const ResourceManager<T, TH>::iterator& other) const {
            return other._handl == this->_handl;
        }

        TH operator*() { return _handl; }

        TH operator->() { return *(*this); }

       private:
        TH _handl;
        ResourceManager<T, TH>& _manager;
    };

    iterator begin() {
        if (_resources.size() > 1)
            return iterator(*this, _resources[1].getHandle());
        return end();
    }

    iterator end() { return iterator(*this, TH(0)); }

   private:
    std::vector<T> _resources;
    std::list<size_t> _free_indexes;
};

}  // namespace gbg
