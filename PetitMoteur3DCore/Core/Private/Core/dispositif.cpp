#include "Core/Public/Core/dispositif.h"

namespace PM3D
{
    void CDispositif::Present()
    {
        PresentSpecific();
    };
} // namespace PM3D
