/*==================================================================================================

                       Copyright (c) 2018 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "IPointCloud.h"
#include "IGridPointCloud.h"
#include <vector>

namespace GPP
{
    class PointCloudInfo;
    class BGL_EXPORT PointCloudNormal
    {
    public:
        PointCloudNormal();
        ~PointCloudNormal();


        // If pointCloud is depth image, result normal will face z positive direction, speed is faster
        // point count >= 4
        // neighborCount >= 4
        // sampleInterval: if pointCloud is non-uniform, user can set sampleInterval to sample pointCloud first
        static ErrorCode Compute(IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo,
            bool isDepthImage = false, Int neighborCount = 9, const Real* sampleInterval = NULL);


        // pointCloud should have normals
        // smoothWeight: (0, 1]. Larger value will smooth more.
        static ErrorCode Smooth(IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo, Real smoothWeight = 0.5,
            Int iterationCount = 1, Int neighborCount = 9, const std::vector<Int>* fixedIndex = NULL);


        // pointCloud should have normal, and this function just update normal and the orientation is the same as before
        static ErrorCode Update(IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo, Int neighborCount);


        // pointCloud should have normal, and this function just update normal and the orientation is the same as before
        static ErrorCode UpdateByRadius(IPointCloud* pointCloud, Real neighborRadius);


        static ErrorCode ComputeSamplePointNormal(const IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo,
            const std::vector<Int>& sampleIndex, std::vector<Vector3>& normalList);


        static ErrorCode ComputeSampleGridNormal(const IGridPointCloud* pointCloud, const std::vector<Int>& sampleIndex,
            std::vector<Vector3>& normalList);


        // fixedIndex: the normal of fixedIndex will be fix, and others' normal will be oriented. If fixedIndex == NULL, all normal will be oriented
        // point count >= 4
        // neighborCount >= 4
        static ErrorCode Orient(IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo,
            Int neighborCount, const std::vector<Int>* fixedIndex);


        static ErrorCode ReversePatch(IPointCloud* pointCloud, Int pointId, Int neighborCount = 9);


        // z should be constant value
        static ErrorCode Compute2D(IPointCloud* pointCloud, bool orientDir = false, Int neighborCount = 9);


        static ErrorCode ComputeGridNormal(IGridPointCloud* pointCloud);


        static ErrorCode SmoothGridNormal(IGridPointCloud* pointCloud, Int neighborWidth, Int iterationCount);

    };
}
