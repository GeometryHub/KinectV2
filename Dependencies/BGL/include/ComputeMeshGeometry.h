/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "ITriMesh.h"
#include "TriMeshInfo.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT ComputeMeshGeometry
    {
    public:
        ComputeMeshGeometry();
        ~ComputeMeshGeometry();

        // positionWeight: (REAL_TOL, +). Larger value will smooth less
        // Normal of result triMesh is not updated here
        // softConstraints: vertices will mark as soft position constraints
        static ErrorCode LaplaceSmooth(ITriMesh* triMesh, bool keepBoundary = true, Real positionWeight = 1.0, 
            const std::vector<Int>* softConstraints = NULL);


        // meshInfo: CacheVertexFaceNeighbors
        static ErrorCode SmoothTriangleNormal(ITriMesh* triMesh, TriMeshInfo* meshInfo, Real sharpAngle, Real normalWeight);


        // intensity: (1.0, +). Larger value will enhance more
        // Normal of result triMesh is not updated here
        static ErrorCode EnhanceDetail(ITriMesh* triMesh, Real intensity = 2.0);


        // sharpAngle is radian: (REAL_TOL, 180 * ONE_RADIAN)
        // positionWeight: (REAL_TOL, +). Larger value will smooth less
        // Normal of result triMesh is not updated here
        static ErrorCode RemoveGeometryNoise(ITriMesh* triMesh, Real sharpAngle = 70.0 * ONE_RADIAN, Real positionWeight = 1.0);


        // if triangle area < REAL_TOL, it will return false
        static bool IsGeometryDegenerate(const ITriMesh* triMesh);


        // minEdgeLength: [REAL_TOL, +)
        // minTriangleAngle is radian: [REAL_TOL, 180 * ONE_RADIAN)
        // foldoverAngleTol is radian: [REAL_TOL, 180 * ONE_RADIAN)
        // Normal of result triMesh is not updated here
        static ErrorCode ConsolidateGeometry(ITriMesh* triMesh, Real minTriangleAngle, Real minEdgeLength, Real foldoverAngleTol);


        static ErrorCode ConsolidateDegenerateAreas(const ITriMesh* triMesh, const std::vector<bool>* vertexFixFlags,
            Real minArea2AvgRatio, std::vector<Vector3>& vertexCoords);


        enum BGL_EXPORT FilterOutType
        {
            FILTEROUT_TYPE_FLAT = 0,
            FILTEROUT_TYPE_SMOOTH
        };

        // selectVertices should not contain boundary vertices
        static ErrorCode FilterOutGeometry(ITriMesh* triMesh, const std::vector<Int>& selectVertices, FilterOutType filterType, Real filterWeight);

    };
}
