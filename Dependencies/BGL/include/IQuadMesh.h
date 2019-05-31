/*==================================================================================================

                       Copyright (c) 2018 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    class BGL_EXPORT IQuadMesh
    {
    public:
        IQuadMesh(){}

        virtual Int GetVertexCount(void) const = 0;
        virtual Int GetQuadCount(void) const = 0;

        virtual Vector3 GetVertexCoord(Int vid) const = 0;
        virtual void SetVertexCoord(Int vid, const Vector3& coord) = 0;
        virtual Vector3 GetVertexNormal(Int vid) const = 0;
        virtual void SetVertexNormal(Int vid, const Vector3& normal) = 0;
        // Sometimes, Mesh doesn't need vertex normal information
        virtual bool HasVertexNormal(void) const = 0;

        // vertexIds are in a consistent order in all connected quads
        virtual void GetQuadVertexIds(Int fid, Int vertexIds[4]) const = 0;
        // make sure vertexIdx are in a consistent order in its connected quads
        virtual void SetQuadVertexIds(Int fid, Int vertexId0, Int vertexId1, Int vertexId2, Int vertexId3) = 0;
        
        // Return inserted quad id
        virtual Int InsertQuad(Int vertexId0, Int vertexId1, Int vertexId2, Int vertexId3) = 0;
        // Return inserted vertex id
        virtual Int InsertVertex(const Vector3& coord) = 0;
        
        // Be careful: if you swap vertex and popback them, then vertex index after the deleted vertices will be changed.
        virtual void SwapVertex(Int vertexId0, Int vertexId1) = 0; 
        virtual void PopbackVertices(Int popCount) = 0;
        virtual void SwapQuads(Int fid0, Int fid1) =0;
        virtual void PopbackQuads(Int popCount) = 0;

        virtual void UpdateNormal(void) = 0;
        // Clear all geometry information to initial state
        virtual void Clear(void) = 0;

        virtual ~IQuadMesh(){};
    };
}
