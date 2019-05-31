/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMeshInfo.h"
#include <vector>

namespace GPP
{
    class TriMeshInfo;
    class BGL_EXPORT Curvature
    {
    public:
        Curvature();
        ~Curvature();

        
        // curvature is related to triMesh's size, if triMesh' size *= s, then curvature /= s;
        static ErrorCode ComputeMeanCurvature(const ITriMesh* triMesh, std::vector<Real>& curvature);


        static ErrorCode ComputeGaussCurvature(const ITriMesh* triMesh, std::vector<Real>& curvature);


        static ErrorCode ComputePrincipalCurvature(const ITriMesh* triMesh, TriMeshInfo* meshInfo, std::vector<Real>* minCurvature,
            std::vector<Real>* maxCurvature, std::vector<Vector3>* minDirs, std::vector<Vector3>* maxDirs);

    };
}
