/*==================================================================================================

                       Copyright (c) 2017s GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "GppDefines.h"
#include "IGridPointCloud.h"
#include <vector>

namespace GPP
{
    class GPP_EXPORT ConsolidateGrid
    {
    public:
        ConsolidateGrid();
        ~ConsolidateGrid();

        
        static ErrorCode CalculateGridNormal(IGridPointCloud* pointCloud);

        
        static ErrorCode CalculateGridIsolation(const IGridPointCloud* pointCloud, std::vector<Real>* isolation, 
            Real maxExtendNormalAngle);

        
        static ErrorCode CalculateGridDensity(const IGridPointCloud* pointCloud, Real& density);


        static ErrorCode DetectGridBoundary(const IGridPointCloud* pointCloud, Int boundarySize, std::vector<std::pair<Int, Int> >& boundaryIds);

        
        // pointCloud should have normal
        static ErrorCode SmoothGridGeometry(IGridPointCloud* pointCloud, Int neighborWidth, Int iterationCount);


        static ErrorCode SmoothGridNormal(IGridPointCloud* pointCloud, Int neighborWidth, Int iterationCount);


        static ErrorCode CalculateBoundingBox(const IGridPointCloud* pointCloud, Vector3& bboxMin, Vector3& bboxMax);

    };
}
