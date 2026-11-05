#pragma once
#include <filesystem>

#include "../DResource.hpp"
#include "../Resource.hpp"
#include "../macros.hpp"

namespace gbg {
namespace fsys = std::filesystem;
RESOURCE_HANDLE(File);
class File : public DResource<FileHandle> {
   public:
    DRESOURCE_CONSTR(File)
    fsys::path path;
};

class FileManager : public ResourceManager<File, FileHandle> {
   public:
    File& create(const fsys::path&& path) {
        auto& res = ResourceManager<File, FileHandle>::create(path.filename());
        res.path = path;
        return res;
    }
};
}  // namespace gbg
