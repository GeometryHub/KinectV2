/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"

namespace GPP
{
    class BGL_EXPORT Profiler
    {
    public:
        Profiler();
        ~Profiler();

        static Real GetTime(void);
    };
}
