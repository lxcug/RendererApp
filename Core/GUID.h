// @author: lx
// @time: 2023/2/25
// @brief: 

#ifndef RENDERER_GUID_H
#define RENDERER_GUID_H

#include "Core.h"
#include <hashtable.h>

namespace RendererSpace {
    class GUID {
    public:
        GUID();
        GUID(uint64_t uuid);
        GUID(const GUID&) = default;

        operator uint64_t() const {
            return m_GUID;
        }
    private:
        uint64_t m_GUID;
    };

}

namespace std {
    template<>
    struct hash<RendererSpace::GUID>
    {
        std::size_t operator()(const RendererSpace::GUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}


#endif //RENDERER_GUID_H
