/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "ITriMesh.h"
#include <vector>

namespace GPP
{
    // Algorithms only need reading triangle mesh information can inherit the ITriangleList interface.
    class BGL_EXPORT ITriangleList
    {
    public:
        ITriangleList(){}
        virtual ~ITriangleList(){}

        virtual Int GetVertexCount(void) const = 0;
        virtual Int GetTriangleCount(void) const = 0;

        virtual Vector3 GetVertexCoord(Int vid) const = 0;
        virtual Vector3 GetVertexNormal(Int vid) const = 0;
        virtual void GetTriangleVertexIds(Int fid, Int vertexIds[3]) const = 0;
        virtual Vector3 GetTriangleNormal(Int fid) const = 0;
    };

    class BGL_EXPORT TriangleList : public ITriangleList
    {
    public:
        explicit TriangleList(const ITriMesh* triMesh);
        virtual ~TriangleList();

        virtual Int GetVertexCount(void) const;
        virtual Int GetTriangleCount(void) const;

        virtual Vector3 GetVertexCoord(Int vid) const;
        virtual Vector3 GetVertexNormal(Int vid) const;
        virtual void GetTriangleVertexIds(Int fid, Int vertexIds[3]) const;
        virtual Vector3 GetTriangleNormal(Int fid) const;
    protected:
        const ITriMesh* mpTriMesh;
    };

    class BGL_EXPORT SubTriangleList : public ITriangleList
    {
    public:
        enum BuildSubTriangleListType
        {
            BUILD_SUBTRIANGLE_LIST_TYPE_BY_TRIANGLES = 0, // only the triangles with the input ids will be in the SubTriMesh
            BUILD_SUBTRIANGLE_LIST_TYPE_BY_VERTICES = 1   // triangle vertex ids are all in will be in the SubTriMesh
        };

        explicit SubTriangleList(const ITriMesh* triMesh, const std::vector<Int>& subEntityIds, 
            BuildSubTriangleListType type = BUILD_SUBTRIANGLE_LIST_TYPE_BY_TRIANGLES);
        virtual ~SubTriangleList();

        virtual Int GetVertexCount(void) const;
        virtual Int GetTriangleCount(void) const;

        virtual Vector3 GetVertexCoord(Int vid) const;
        virtual Vector3 GetVertexNormal(Int vid) const;
        virtual void GetTriangleVertexIds(Int fid, Int vertexIds[3]) const;
        virtual Vector3 GetTriangleNormal(Int fid) const;

        // these function will do the id transfer
        Int VertexIdToSubTriMeshId(Int idAgainstITriMesh)const;
        Int SubTriMeshIdToVertexId(Int idAgaintSubTriMesh)const;

    private:
        void BuildSubTriMeshByTriangles(const std::vector<Int>& subTriangleIds);
        void BuildSubTriMeshByVertices(const std::vector<Int>& vertexIds);

    private:
        const ITriMesh* mpTriMesh;
        std::vector<Int> mSubTriangleIds;
        std::vector<Int> mSubVertexIds;
        std::vector<Int> mVertexIdToSubTriMeshMap;
    };
}