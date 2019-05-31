/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMesh.h"
#include "Plane3.h"
#include <vector>

namespace GPP
{
    class TriMeshInfo;
    class BGL_EXPORT SplitMesh
    {
    public:
        SplitMesh();
        ~SplitMesh();

        // splitted triMesh's normal is not updated here
        // splitLines.at(lineid) is a line vertex list
        // insertVertexIdMap->at(insertedVertexId - originVertexCount) = originVertexId
        static ErrorCode SplitByLines(ITriMesh* triMesh, const std::vector<std::vector<Int> >& splitLines,
            std::vector<Int>* insertVertexIdMap = NULL);


        static ErrorCode SplitByFaceIds(ITriMesh* triMesh, const std::vector<Int>& faceIds, std::vector<Int>* insertVertexIdMap = NULL);


        // triMesh: new triangles will be added near the cutting plane, since plane will cut triangle into sub-triangles
        // triangleFlags: 1 - above plane, 0 - under plane
        static ErrorCode SplitByPlane(ITriMesh* triMesh, const Plane3* plane, std::vector<bool>* triangleFlags,
            const std::vector<Real>* vertexFields = NULL, std::vector<Real>* insertedVertexFields = NULL);

    };
}
