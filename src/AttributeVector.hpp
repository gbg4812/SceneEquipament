#pragma once

#include <vector>

namespace gbg {
template <typename T>
class AttributeVector {
   public:
    AttributeVector() {}
    AttributeVector(AttributeVector&&) = default;
    AttributeVector(const AttributeVector&) = delete;
    AttributeVector& operator=(AttributeVector&&) = delete;
    AttributeVector& operator=(const AttributeVector&) = delete;
    ~AttributeVector();

   private:
    std::vector<T>& _attribute;
};

}  // namespace gbg
