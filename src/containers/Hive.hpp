#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

template <typename T>
class Hive {
   public:
    Hive(size_t chunk_size = 64) : _chunk_size(chunk_size) {};

    size_t push_back(T&& value) {
        if (_size >= _chunk_size * _blocks.size()) {
            _blocks.push_back(std::make_unique<T[]>(_chunk_size));
        }
        int block = _size / _chunk_size;
        int idx = _size % _chunk_size;
        _blocks[block][idx] = std::move(value);
        return _size++;
    }

    size_t push_back(const T& value) {
        if (_size >= _chunk_size * _blocks.size()) {
            _blocks.push_back(std::make_unique<T[]>(_chunk_size));
        }
        int block = _size / _chunk_size;
        int idx = _size % _chunk_size;
        _blocks[block][idx] = value;
        return _size++;
    }

    void set(size_t idx, const T& value) {
        if (idx >= _size) {
            throw std::out_of_range("Idx out of range (Hive)");
        }

        int block = _size / _chunk_size;
        int _idx = _size % _chunk_size;
        return _blocks[block][_idx] = value;
    }

    T& at(size_t idx) {
        if (idx >= _size) {
            throw std::out_of_range("Idx out of range (Hive)");
        }

        size_t block = idx / _chunk_size;
        size_t _idx = idx % _chunk_size;
        return _blocks[block][_idx];
    }

    void reserve(size_t size) {
        int diff = size - (_chunk_size * _blocks.size());
        if(diff <=0) return;
        size_t blocks = diff / _chunk_size;
        blocks += (diff % _chunk_size > 0);
        _blocks.reserve(blocks);
        for (size_t i = 0; i < blocks; i++) {
            _blocks.push_back(std::make_unique<T[]>(_chunk_size));
        }
    }

    void resize(size_t size) {
        reserve(size);
        _size = size;
    }

    T& operator[](size_t idx) { return at(idx); }
    const T& operator[](size_t idx) const { return at(idx); }

    size_t size() const { return _size; };

    class iterator {
       public:
        iterator(Hive* container, size_t index = 0)
            : _this(container), _index(index) {};

        bool operator==(const iterator& other) const {
            return (_this == other._this) && (_index == other._index);
        }

        iterator& operator++() {
            _index = _index < _this->size() ? _index++ : _index;
            return *this;
        }

        iterator operator++(int) const { return ++(*this); }

       private:
        Hive* _this = nullptr;
        size_t _index = 0;
    };

    iterator begin() { return iterator(this); };
    iterator end() { return iterator(this, size()); };

   private:
    const int _chunk_size;
    int _size = 0;
    std::vector<std::unique_ptr<T[]>> _blocks;
};
