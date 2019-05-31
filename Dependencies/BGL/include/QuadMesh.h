/*==================================================================================================

                       Copyright (c) 2018 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "IQuadMesh.h"
#include <vector>

namespace GPP
{
    struct BGL_EXPORT QuadInfo
    {
        QuadInfo(void);
        QuadInfo(Int vertexId0, Int vertexId1, Int vertexId2, Int vertexId3);
        Int mIndex[4];
    };


    class BGL_EXPORT QuadMesh : public IQuadMesh
    {
    public:
        QuadMesh();

        virtual Int GetVertexCount(void) const;
        virtual Int GetQuadCount(void) const;

        virtual Vector3 GetVertexCoord(Int vid) const;
        virtual void SetVertexCoord(Int vid, const Vector3& coord);
        virtual Vector3 GetVertexNormal(Int vid) const;
        virtual void SetVertexNormal(Int vid, const Vector3& normal);
        virtual bool HasVertexNormal(void) const;

        // vertexIds are in a consistent order in all connected Quads
        virtual void GetQuadVertexIds(Int fid, Int vertexIds[4]) const;
        // make sure vertexIdx are in a consistent order in its connected quads
        virtual void SetQuadVertexIds(Int fid, Int vertexId0, Int vertexId1, Int vertexId2, Int vertexId3);
        
        // Return inserted quad id
        virtual Int InsertQuad(Int vertexId0, Int vertexId1, Int vertexId2, Int vertexId3);
        // Return inserted vertex id
        virtual Int InsertVertex(const Vector3& coord);
        
        // Be careful: if you swap vertex and popback them, then vertex index after the deleted vertices will be changed.
        virtual void SwapVertex(Int vertexId0, Int vertexId1); 
        virtual void PopbackVertices(Int popCount);
        virtual void SwapQuads(Int fid0, Int fid1);
        virtual void PopbackQuads(Int popCount);

        virtual void UpdateNormal(void);
        // Clear all geometry information to initial state
        virtual void Clear(void);

        virtual ~QuadMesh();

    private:
        std::vector<Vector3> mVertexCoordList;
        std::vector<Vector3> mVertexNormalList;
        std::vector<Vector3> mVertexColorList;
        std::vector<QuadInfo*> mQuadList;
        bool mHasVertexNormal;
        bool mHasVertexColor;
        Vector3 mDefaultColor;
    };

}