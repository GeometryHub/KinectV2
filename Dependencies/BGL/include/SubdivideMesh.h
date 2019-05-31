/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "TriMesh.h"
#include <vector>

namespace GPP
{

    class BGL_EXPORT SubdivideMesh
    {
    public:
        SubdivideMesh();
        ~SubdivideMesh();

        // vertexFields: it could be color, texture coordinate....
        // Normal of result triMesh is not updated here
        static ErrorCode LoopSubdivideMesh(ITriMesh* triMesh, const std::vector<Real>* vertexFields = NULL, std::vector<Real>* insertedVertexFields = NULL);


        // vertexFields: it could be color, texture coordinate....
        static ErrorCode DensifyMesh(ITriMesh* triMesh, Int targetVertexCount, const std::vector<Real>* vertexFields, std::vector<Real>* insertedVertexFields);


        // insertVertexCoords: insert vertices' coordinate in original triMesh. 
        static ErrorCode DensifyMesh(ITriMesh* triMesh, Int targetVertexCount, std::vector<std::vector<PointOnFace> >* insertVertexCoords);


        // insertVertexCoords: insert vertices' coordinate in original triMesh. 
        static ErrorCode DensifyMeshByLength(ITriMesh* triMesh, Real maxEdgeLength, std::vector<std::vector<PointOnFace> >* insertVertexCoords);


        // pointsOnFace or pointsOnVertex should not be on vertex
        // Tesselated mesh vertex will be in order: triMesh, pointsOnFace, pointsOnEdge
        // triMesh's normal is not updated
        static ErrorCode MutualTessellation(ITriMesh* triMesh, const std::vector<PointOnFace>* pointsOnFace,
            const std::vector<PointOnEdge>* pointsOnEdge, std::vector<bool>* pointValidFlags);
    };
}
