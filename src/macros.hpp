#pragma once

#define RESOURCE_CONSTR(ResourceTypeName) \
    ResourceTypeName() : Resource<ResourceTypeName##Handle>(){};    \
    ResourceTypeName(std::string name, uint32_t rid) : Resource(name, rid){};

#define DRESOURCE_CONSTR(ResourceTypeName) \
    ResourceTypeName() : DResource(){};    \
    ResourceTypeName(std::string name, uint32_t rid) : DResource(name, rid){};

#define RESOURCE_HANDLE(ResourceTypeName)                         \
    struct ResourceTypeName##Handle : public ResourceHandle {     \
       public:                                                    \
        ResourceTypeName##Handle() : ResourceHandle(){};          \
        ResourceTypeName##Handle(uint32_t rid)      \
            : ResourceHandle(rid){};                       \
        ResourceTypeName##Handle(const ResourceHandle& handle) : ResourceHandle(handle){\
        \
        }; \
    }

#define RESOURCE_MANAGER(ResourceTypeName)                              \
    typedef ResourceManager<ResourceTypeName, ResourceTypeName##Handle> \
        ResourceTypeName##Manager;
