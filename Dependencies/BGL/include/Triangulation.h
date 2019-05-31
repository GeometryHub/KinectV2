/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "Vector2.h"
#include <vector>

namespace GPP
{

    class BGL_EXPORT Triangulation
    {
    public:
        Triangulation();
        ~Triangulation();


        static ErrorCode ConstrainedDelaunay2D(const std::vector<Vector2>& inputPointList, 
            const std::vector<std::vector<Int> >* polylineList, 
            const std::vector<std::vector<Int> >* directPolylineList,
            std::vector<Int>& triangleList, std::vector<Int>* noUsePointList);

    };
}
