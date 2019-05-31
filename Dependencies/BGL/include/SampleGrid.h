/*==================================================================================================

                       Copyright (c) 2017 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "IGridPointCloud.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT SampleGrid
    {
    public:
        SampleGrid();
        ~SampleGrid();


        // sampleIndex should be allocated memory before calling this api
        static ErrorCode UniformSample(const IGridPointCloud* pointCloud, Int sampleCount, 
            std::vector<std::pair<Int, Int> >* sampleIndex);


        // pointCloud should have normal information
        // uniformWeight: [0, 1]. Larger value will get a more uniform result
        // planeRatio: return value, which means planePointCount / pointCount
        static ErrorCode GeometrySample(const IGridPointCloud* pointCloud, Int sampleCount, Real uniformWeight, 
            std::vector<std::pair<Int, Int> >* sampleIndex, Real* planeRatio);


        enum GridSimplifyType
        {
            GRID_SIMPLIFY_SAMPLE = 0,
            GRID_SIMPLIFY_MERGE
        };
        // simplifiedCloud should allocate memory first
        static ErrorCode Simplify(const IGridPointCloud* pointCloud, Real interval, GridSimplifyType type, 
            IGridPointCloud* simplifiedCloud);

    };
}
