/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "IPointList.h"
#include "PointCloudInfo.h"
#include <vector>

namespace GPP
{
    enum SampleQuality
    {
        SAMPLE_QUALITY_LOW = 0,
        SAMPLE_QUALITY_HIGH
    };

    class BGL_EXPORT SamplePointCloud
    {
    public:
        SamplePointCloud();
        ~SamplePointCloud();


        static ErrorCode UniformSample(const IPointList* pointList, Int sampleCount, std::vector<Int>& sampleIndex,
            Int seedId = 0, SampleQuality quality = SAMPLE_QUALITY_HIGH);


        // pointCloud should have normal information
        // uniformWeight: [0, 1]. Larger value will get a more uniform result
        // planeRatio: return value, which means planePointCount / pointCount
        static ErrorCode GeometrySample(const IPointList* pointList, Int sampleCount, std::vector<Int>& sampleIndex,
            Real uniformWeight = 0.1, Int neighborCount = 9, Int seedId = 0, SampleQuality quality = SAMPLE_QUALITY_HIGH,
            Real* planeRatio = NULL);


        static ErrorCode GridSample(const IPointList* pointList, Real interval, std::vector<Int>& sampleIndex);


        static ErrorCode RandSample(Int pointCount, Int sampleCount, const std::vector<Real>* pointWeights, std::vector<Int>& sampleIndex);


    };
}
