#include "DResource.hpp"
#include "macros.hpp"
namespace gbg {

    RESOURCE_HANDLE(Probe);
    
    enum class ProbeType {
        REFLECTION,
    };
    
    class Probe : DResource<ProbeHandle> {
        DRESOURCE_CONSTR(Probe)

        public:
        ProbeType type;
    };

    RESOURCE_MANAGER(Probe);
}
