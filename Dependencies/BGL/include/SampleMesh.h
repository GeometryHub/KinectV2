/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMesh.h"
#include "IPointCloud.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT SampleMesh
    {
    public:
        SampleMesh();
        ~SampleMesh();

        // points on the same face or edge are neighbors in pointsOnFace and pointsOnEdge
        // weights of points on the same edge are in ascending order 
        // sharpAngle: if edge faces' angle is larger than sharpAngle, the edge will be sharp edge. sharpAngle range (0, 180 * ONE_RADIAN)
        // pointsOnFace and pointsOnEdge should be strictly in face or edge, not on the boundary
        static ErrorCode UniformSample(const ITriMesh* triMesh, Int sampleCount, Real sharpAngle, std::vector<PointOnFace>& pointsOnFace, 
            std::vector<PointOnEdge>& pointsOnEdge, std::vector<Int>& pointsOnVertex, bool excludeBoundary);



        // triMesh should have vertex normal
        // points on the same face or edge are neighbors in pointsOnFace and pointsOnEdge
        // weights of points on the same edge are in ascending order 
        // sharpAngle: if edge faces' angle is larger than sharpAngle, the edge will be sharp edge. sharpAngle range (0, 180 * ONE_RADIAN)
        // pointsOnFace and pointsOnEdge should be strictly in face or edge, not on the boundary
        // uniformWeight: [0, 1]. Larger value will get a more uniform result
        static ErrorCode GeometrySample(const ITriMesh* triMesh, Int sampleCount, Real sharpAngle, Real uniformWeight,
            std::vector<PointOnFace>& pointsOnFace, std::vector<PointOnEdge>& pointsOnEdge, std::vector<Int>& pointsOnVertex, 
            bool excludeBoundary);

    };

    extern BGL_EXPORT ErrorCode ConstructSamplePointCloud(IPointCloud* pointCloud, const ITriMesh* triMesh,
        const std::vector<PointOnFace>& pointsOnFace, const std::vector<PointOnEdge>& pointsOnEdge, const std::vector<Int>& pointsOnVertex);
}
