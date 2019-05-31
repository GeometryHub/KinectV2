/*==================================================================================================

                       Copyright (c) 2017 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMesh.h"
#include <set>

namespace GPP
{
    class BGL_EXPORT TriMeshInfo
    {
    public:
        TriMeshInfo();
        ~TriMeshInfo();

        ErrorCode CacheEdgeInfos(const ITriMesh* triMesh, bool oneWay);
        void ClearEdgeInfos();
        bool IsOneWayEdgeInfo()const;
        const std::vector<std::map<Int, Int> >& GetVertexEdgeMaps()const;
        std::vector<std::map<Int, Int> >& GetVertexEdgeMaps();
        const std::vector<EdgeInfo>& GetEdgeInfoList()const;
        std::vector<EdgeInfo>& GetEdgeInfoList();

        ErrorCode CacheVertexVertexNeighbors(const ITriMesh* triMesh);
        void ClearVertexVertexNeighbors();
        const std::vector<std::set<Int> >& GetVertexVertexNeighbors()const;
        std::vector<std::set<Int> >& GetVertexVertexNeighbors();

        ErrorCode CacheVertexFaceNeighbors(const ITriMesh* triMesh);
        void ClearVertexFaceNeighbors();
        const std::vector<std::vector<Int> >& GetVertexFaceNeighbors()const;
        std::vector<std::vector<Int> >& GetVertexFaceNeighbors();

        ErrorCode CacheFaceFaceNeighbors(const ITriMesh* triMesh);
        void ClearFaceFaceNeighbors();
        const std::vector<std::vector<Int> >& GetFaceFaceNeighbors() const;
        std::vector<std::vector<Int> >& GetFaceFaceNeighbors();


        void Clear(void);

    private:
        bool mIsOneWayEdgeInfo;
        std::vector<EdgeInfo> mEdgeInfoList;
        std::vector<std::map<Int, Int> > mVertexEdgeMap;

        std::vector<std::set<Int> > mVertexVertexNeighbor;

        std::vector<std::vector<Int> > mVertexFaceNeighbors;

        std::vector<std::vector<Int> > mFaceFaceNeighbors;

    };
}