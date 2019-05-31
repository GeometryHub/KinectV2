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
    class BGL_EXPORT ComputeMeshTopology
    {
    public:
        ComputeMeshTopology();
        ~ComputeMeshTopology();

        
        // invalidVertexId: the first invalid vertex id encountered
        static bool IsTriMeshManifold(const ITriMesh* triMesh, Int* invalidVertexId = NULL);


        // meshInfo: CacheEdgeInfos, oneWay == false
        // boundarySeedIds: only holes containing vertices in boundarySeedIds will be found. If none, it will find all holes
        // holdsIds are all oriented.
        static ErrorCode FindMeshHoles(const ITriMesh* triMesh, TriMeshInfo* meshInfo, const std::vector<Int>* boundarySeedIds,
            std::vector<std::vector<Int> > *holesIds);


        // isolation value is between [0, 1], smaller value means more isolated
        // meshInfo: CacheVertexVertexNeighbors
        static ErrorCode CalculateIsolation(const ITriMesh* triMesh, TriMeshInfo* meshInfo, std::vector<Real>* isolation);


        // meshInfo: CacheVertexVertexNeighbors
        static ErrorCode CalculateConnectedRegions(const ITriMesh* triMesh, TriMeshInfo* meshInfo, const std::vector<Int>* vertexSeeds,
            std::vector<Int>* connectedRegions);


        // removingVertices: need removing vertex index
        // vertexMap: simplified vertex index map to origin vertex index
        // targetVertexCount: controling simplified vertex count. Usually *targetVertexCount == vertexCount - removingVertices.size()
        // sharpAngle: if edge faces' angle is larger than sharpAngle, the edge will be sharp edge and its vertex position will be fixed. 
        // sharpAngle range (0, 180 * ONE_RADIAN)
        // vertexSharpFlags: vertex flag on sharp edge is 1; sharp edge is defined from sharpAngle
        static ErrorCode SimplifyByRemovingVertex(ITriMesh* triMesh, const std::vector<Int>& removingVertices,
            std::vector<Int>* vertexMap, const Int* targetVertexCount, const Real* sharpAngle,
            std::vector<bool>* vertexSharpFlags);
    };
}
