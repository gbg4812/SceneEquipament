#include <cstdint>

namespace gbg {
struct DepDataHandle {
    uint32_t index;
    uint16_t rid;
    uint8_t type;
    uint8_t flags;
};
}  // namespace gbg
