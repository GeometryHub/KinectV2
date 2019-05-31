/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "ITriMesh.h"

namespace GPP
{
    class BGL_EXPORT OffsetMesh
    {
    public:
        OffsetMesh();
        ~OffsetMesh();

        // Offset mesh will be added into triMesh, if there are boundaries, boundaries will be connected
        // fabs(offsetValue) > REAL_TOL
        static ErrorCode UniformApproximate(ITriMesh* triMesh, Real offsetValue);
    };
}
