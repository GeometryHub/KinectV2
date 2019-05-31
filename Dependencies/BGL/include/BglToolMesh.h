/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector2.h"
#include "TriMesh.h"
#include "QuadMesh.h"
#include "Matrix4x4.h"

namespace GPP
{
    // inputMesh should not be null
    // convertedMesh's normal is not updated here
    extern BGL_EXPORT ErrorCode ConvertTriMeshToITriMesh(const TriMesh* inputMesh, ITriMesh* convertedMesh);


    // Deep Copy
    // vertex coord, normal, color, textureCoord; triangle id, normal, color, textureCoord
    extern BGL_EXPORT TriMesh* CopyTriMesh(const TriMesh* triMesh);


    // Deep Copy
    // vertex coord, normal; triangle id, normal
    extern BGL_EXPORT ErrorCode CopyTriMesh(const ITriMesh* triMesh, ITriMesh* copyMesh);
    
    
    // Related triangles will also be deleted
    // triMesh's normal is not updated here
    extern BGL_EXPORT ErrorCode DeleteTriMeshVertices(ITriMesh* triMesh, const std::vector<Int>& deleteIndex);


    // triMesh's normal is not updated here
    // deleteIsolatedVertices: after deleting triangles, there may exist isolated vertices (degree is zero), if true, isolated vertices will be deleted
    extern BGL_EXPORT ErrorCode DeleteTriMeshTriangles(ITriMesh* triMesh, const std::vector<Int>& deleteIndex, bool deleteIsolatedVertices);


    // triMesh's normal is not updated here
    // This api will delete vertices whose degree is zero
    extern BGL_EXPORT ErrorCode DeleteIsolatedVertices(ITriMesh* triMesh);


    extern BGL_EXPORT ErrorCode CalculateAverageEdgeLength(const ITriMesh* triMesh, Real& avgEdgeLength);


    extern BGL_EXPORT ErrorCode CalculateTriMeshBoundingBox(const ITriMesh* triMesh, Vector3& bboxMin, Vector3& bboxMax);
    
    
    class HalfMesh;
    // returned halfmesh's normal is not updated here
    ////////////////////////////////////////////////////////////////////////////////////// 
    // note, face vertex id order: 
    // triMesh->GetTriangleVertexIds(fid, vertexIds);
    // halfMesh->GetFace(fid)->GetEdge()->GetVertex() == triMesh->GetVertex(vertexIds[0]);
    ////////////////////////////////////////////////////////////////////////////////////// 
    extern BGL_EXPORT HalfMesh* CreateHalfMeshFromITriMesh(const ITriMesh* triMesh);

    
    // returned halfmesh's normal is not updated here
    extern BGL_EXPORT HalfMesh* CreateHalfMeshFromSubTriMesh(const ITriMesh* triMesh, const std::vector<Int>& triangles, 
        std::vector<Int>* origin2SubMap);


    // triMesh should not be null
    // halfMesh should UpdateVertexIndex before calling
    // triMesh's normal is not updated here
    ////////////////////////////////////////////////////////////////////////////////////// 
    // note, face vertex id order: 
    // triMesh->GetTriangleVertexIds(fid, vertexIds);
    // halfMesh->GetFace(fid)->GetEdge()->GetVertex() == triMesh->GetVertex(vertexIds[0]);
    ////////////////////////////////////////////////////////////////////////////////////// 
    extern BGL_EXPORT ErrorCode ConvertHalfMeshToITriMesh(const HalfMesh* halfMesh, ITriMesh* triMesh);


    extern BGL_EXPORT ErrorCode ComputeTriangleNormal(const ITriMesh* triMesh, std::vector<Vector3>& normalList);


    extern BGL_EXPORT Real ComputeTriMeshArea(const ITriMesh* triMesh);


    extern BGL_EXPORT ErrorCode ComputeTriangleAreas(const ITriMesh* triMesh, std::vector<Real>& areaList);


    extern BGL_EXPORT Real TriangleArea(const Vector3& triangleV0, const Vector3& triangleV1, const Vector3& triangleV2);


    // Note: User should be aware that this function can ONLY calculte watertight mesh which has no self-intersection. 
    // Otherwise, the result will have deviation.
    extern BGL_EXPORT Real ComputeTriMeshVolume(const ITriMesh* triMesh);


    extern BGL_EXPORT Vector3 ComputeTriMeshBarycentre(const ITriMesh* triMesh);


    extern BGL_EXPORT void TransformTriMesh(ITriMesh* triMesh, const Matrix4x4& transform);


    extern BGL_EXPORT TriMesh* ConvertQuadMesh2TriMesh(const QuadMesh* quadMesh);


    // oneWayMap: if true, every edge will be in vertexEdgeMap only once like vertexEdgeMap[smallVertexId][largeVertexId]
    // selectVertices: only select vertices will construct edge info
    extern BGL_EXPORT ErrorCode ConstructEdgeInfo(const ITriMesh* triMesh, std::vector<EdgeInfo>& edgeInfoList,
        std::vector<std::map<Int, Int> >* vertexEdgeMap = NULL, bool oneWayMap = true, std::vector<Int>* vertexValences = NULL,
        const std::vector<Int>* selectVertices = NULL);


    // edgeInfoList, vertexEdgeMap are needed for input, and they will be modified after vertex is inserted
    extern BGL_EXPORT ErrorCode InsertVertexOnEdge(ITriMesh* triMesh, std::vector<EdgeInfo>& edgeInfoList,
        std::vector<std::map<Int, Int> >& vertexEdgeMap, bool oneWayMap, Int startVertexId, Int endVertexId,
        const Vector3& insertCoord, Int* insertVertexId, std::vector<Int>* insertEdgeIds);


}
