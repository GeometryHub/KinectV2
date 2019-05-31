/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "IPointList.h"
#include <vector>

namespace GPP
{
    class NNQuery;
    class BGL_EXPORT PointCloudInfo
    {
    public:
        PointCloudInfo();
        ~PointCloudInfo();

        // InitAnn will allocate memory for mNeighborIndex and mNeighborDistance
        // Usage: 
        // 1. InitNNQurey 
        // 2. ComputeNeighbors(optional)
        // 3. GetNeighbor
        ErrorCode InitNNQuery(const IPointList* pointList, Int neighborCount);
        NNQuery* GetNNQuery(void);
        ErrorCode ComputeNeighbors(Int neighborCount, const IPointList* pointList, const std::vector<Int>* selectIndex = NULL);
        ErrorCode GetNeighbor(Int pointId, Int neighborCount, Int** index, Real** distance, const IPointList* pointList);
        void ClearNeighborInfo(void);

        void Clear(void);

    private:
        NNQuery* mpNNQuery;
        Int mNeighborCount;
        std::vector<Int*> mNeighborIndex;
        std::vector<Real*> mNeighborDistance;
    };
}
