
#include <filesystem>

#include "../DResource.hpp"
#include "../macros.hpp"

namespace gbg {
namespace fsys = std::filesystem;
RESOURCE_HANDLE(File);
class File : public DResource<FileHandle> {
   public:
    DRESOURCE_CONSTR(File)
    fsys::path path;
};

RESOURCE_MANAGER(File)
}  // namespace gbg
