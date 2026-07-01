#pragma once

#define RESOURCE_CONSTR(ResourceTypeName) \
    ResourceTypeName() : Resource(){};    \
    ResourceTypeName(std::string name, uint32_t rid) : Resource(name, rid){};

#define RESOURCE_HANDLE(ResourceTypeName)                     \
    struct ResourceTypeName##Handle : public ResourceHandle { \
       public:                                                \
        ResourceTypeName##Handle() : ResourceHandle(){};      \
        ResourceTypeName##Handle(uint32_t rid, size_t index)  \
            : ResourceHandle(rid, index){};                   \
    }

#define CREATE_AND_GET(instance, manager, name) \
    auto instance##_h = manager.create(name);                 \
    auto& instance = manager.get(instance##_h);

#define RESOURCE_MANAGER(ResourceTypeName)                              \
    typedef ResourceManager<ResourceTypeName, ResourceTypeName##Handle> \
        ResourceTypeName##Manager;
