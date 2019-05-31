/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    class BGL_EXPORT ITriMesh
    {
    public:
        ITriMesh(){}

        virtual Int GetVertexCount(void) const = 0;
        virtual Int GetTriangleCount(void) const = 0;

        virtual Vector3 GetVertexCoord(Int vid) const = 0;
        virtual void SetVertexCoord(Int vid, const Vector3& coord) = 0;
        virtual Vector3 GetVertexNormal(Int vid) const = 0;
        virtual void SetVertexNormal(Int vid, const Vector3& normal) = 0;
        // Sometimes, Mesh doesn't need vertex normal information
        virtual bool HasVertexNormal(void) const = 0;

        // vertexIds are in a consistent order in all connected triangles
        virtual void GetTriangleVertexIds(Int fid, Int vertexIds[3]) const = 0;
        // make sure vertexIdx are in a consistent order in its connected triangles
        virtual void SetTriangleVertexIds(Int fid, Int vertexId0, Int vertexId1, Int vertexId2) = 0;
        virtual Vector3 GetTriangleNormal(Int fid) const = 0;
        virtual void SetTriangleNormal(Int fid, const Vector3& normal) = 0;
        // Sometimes, Mesh doesn't need triangular normal information
        virtual bool HasTriangleNormal(void) const = 0;
        
        // Return inserted triangle id
        virtual Int InsertTriangle(Int vertexId0, Int vertexId1, Int vertexId2) = 0;
        // Return inserted vertex id
        virtual Int InsertVertex(const Vector3& coord) = 0;
        
        // Be careful: if you swap vertex and popback them, then vertex index after the deleted vertices will be changed.
        // If you want to delete some vertices, please use api DeleteTriMeshVertex in BglToolMesh.h which is still developping.
        virtual void SwapVertex(Int vertexId0, Int vertexId1) = 0; 
        virtual void PopbackVertices(Int popCount) = 0;
        virtual void SwapTriangles(Int fid0, Int fid1) =0;
        virtual void PopbackTriangles(Int popCount) = 0;

        virtual void UpdateNormal(void) = 0;
        // Clear all geometry information to initial state
        virtual void Clear(void) = 0;

        virtual ~ITriMesh(){};
    };
}
