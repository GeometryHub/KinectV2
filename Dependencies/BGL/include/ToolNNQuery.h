/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "IPointList.h"
#include "IGridPointCloud.h"
#include "Matrix4x4.h"
#include <vector>

namespace GPP
{
    class NNKdTree;
    class BGL_EXPORT NNQuery
    {
    public:
        NNQuery();
        ~NNQuery();

        ErrorCode Init(const Real* refData, Int refCount, Int refDim);


        // If you want to use IPointCloud or ITriMesh here, please use PointCloudPointList, TriMeshPointList instead
        ErrorCode Init(const IPointList* pointList, const Matrix4x4* transform = NULL);


        // squareDistRes are sorted in increasing order
        // squareDistRes: Be careful, it is Square Length!
        // if indexRes or squareDistRes is not needed, please set it as NULL
        ErrorCode FindKNN(const Real* searchData, Int searchCount, Int neighborCount, 
            Int* indexRes, Real* squareDistRes) const;


        // squareDistRes are sorted in increasing order
        // squareDistRes: Be careful, it is Square Length!
        // if indexRes or squareDistRes is not needed, please set it as NULL
        ErrorCode FindKNN(const IPointList* pointList, const Matrix4x4* transform, Int neighborCount,
            Int* indexRes, Real* squareDistRes) const;


        // squareDistRes are not sorted!!!
        // squareRadius: Be careful, squared radius of query ball
        // squareDistRes: Be careful, it is Square Length!
        // if indexRes or squareDistRes is not needed, please set it as NULL
        ErrorCode FindRadiusNN(const Real* searchData, Real squareRadius,
            std::vector<Int>* indexRes, std::vector<Real>* squareDistRes) const;


        // squareRadius: Be careful, squared radius of query ball
        ErrorCode ComputeRadiusNNSize(const Real* searchData, Real squareRadius, Int& nnSize) const;


        void Clear(void);


    private:
        NNKdTree* mpKdTree;
        Int mDataDim;
    };
}
