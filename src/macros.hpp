#pragma once

#define RESOURCE_CONSTR(ResourceTypeName)             \
    ResourceTypeName() : Resource(){};                \
    ResourceTypeName(uint32_t rid) : Resource(rid){}; \
    ResourceTypeName(std::string name, uint32_t rid) : Resource(name, rid){};

#define DRESOURCE_CONSTR(ResourceTypeName)             \
    ResourceTypeName() : DResource(){};                \
    ResourceTypeName(uint32_t rid) : DResource(rid){}; \
    ResourceTypeName(std::string name, uint32_t rid) : DResource(name, rid){};

#define RESOURCE_HANDLE(ResourceTypeName)                               \
    struct ResourceTypeName##Handle : public ResourceHandle {           \
       public:                                                          \
        ResourceTypeName##Handle() : ResourceHandle(){};                \
        ResourceTypeName##Handle(uint32_t rid) : ResourceHandle(rid){}; \
        ResourceTypeName##Handle(uint32_t gen, uint32_t idx)            \
            : ResourceHandle(gen, idx){};                               \
    }

#define RESOURCE_MANAGER(ResourceTypeName)                              \
    typedef ResourceManager<ResourceTypeName, ResourceTypeName##Handle> \
        ResourceTypeName##Manager;

#define RELATED_RESOURCE_MANAGER(ResourceTypeName, HandleType) \
    typedef ResourceManager<ResourceTypeName, HandleType>      \
        ResourceTypeName##Manager;
