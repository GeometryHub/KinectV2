/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMeshInfo.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT MeshCutCurve
    {
    public:
        MeshCutCurve();
        ~MeshCutCurve();

        // meshInfo: CacheVertexVertexNeighbors
        static ErrorCode GenerateGaussianSingularity(const ITriMesh* triMesh, TriMeshInfo* meshInfo, Int maxSingularityCount,
            std::vector<Int>* singularityList);


        static ErrorCode GenerateConeSingularity(const ITriMesh* triMesh, Int maxSingularityCount,
            std::vector<Int>* singularityList);


        // meshInfo: CacheVertexVertexNeighbors
        static ErrorCode GenerateSingularitySplitLines(const ITriMesh* triMesh, TriMeshInfo* meshInfo,
            const std::vector<Int>& singularityList, std::vector<std::vector<Int> >* splitLines);


        // initChartCount: triMesh will be segmented into initChartCount part before splitting
        // meshInfo: CacheEdgeInfos, oneWay == true
        static ErrorCode GenerateAtlasSplitLines(const ITriMesh* triMesh, TriMeshInfo* meshInfo, Int initChartCount, std::vector<std::vector<Int> >& splitLines);
    };
}
