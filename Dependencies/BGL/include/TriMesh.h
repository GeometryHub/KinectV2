/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "ITriMesh.h"
#include "PointCloud.h"
#include <vector>
#include <map>

namespace GPP
{
    struct BGL_EXPORT TriangleInfo
    {
        TriangleInfo(void);
        TriangleInfo(Int vertexId0, Int vertexId1, Int vertexId2);
        Int mIndex[3];
    };

    enum MeshType
    {
        MT_NORMAL = 0,
        MT_TRIANGLE_SOUP
    };

    /* Normal infomation: In default setting£¬mesh has no vertex and triangle normal. 
       VertexNormal: InsertVertex(const Vector3& coord, const Vector3& normal), SetVertexNormal and UpdateNormal will make mesh has vertex normal
       TriangleNormal: SetTriangleNormal and UpdateNormal will make mesh has triangle normal
    */
    class BGL_EXPORT TriMesh : public ITriMesh
    {
    public:
        // In the default constructor, TriMesh has no color, vertex texture coordinate, triangle texture coordinate information
        // User can set the information in the constructor or use SetHas** api to allocate the memory
        TriMesh();
        TriMesh(bool hasVertexColor, bool hasVertexTexCoord, bool hasTriangleTexCoord);

        virtual Int GetVertexCount(void) const;
        virtual Int GetTriangleCount(void) const;

        virtual Vector3 GetVertexCoord(Int vid) const;
        virtual void SetVertexCoord(Int vid, const Vector3& coord);
        virtual Vector3 GetVertexNormal(Int vid) const;
        virtual void SetVertexNormal(Int vid, const Vector3& normal);
        virtual bool HasVertexNormal(void) const;

        virtual void GetTriangleVertexIds(Int fid, Int vertexIds[3]) const;
        virtual void SetTriangleVertexIds(Int fid, Int vertexId0, Int vertexId1, Int vertexId2);
        virtual Vector3 GetTriangleNormal(Int fid) const;
        virtual void SetTriangleNormal(Int fid, const Vector3& normal);
        virtual bool HasTriangleNormal(void) const;

        // Return inserted triangle id
        // If mesh has normal, color, texture attribute, this function will insert default value to attribute list to fill the memory space.
        // User can use Set*** function to set attribute value
        virtual Int InsertTriangle(Int vertexId0, Int vertexId1, Int vertexId2);
        
        // Return inserted vertex id
        // If mesh has normal, color, texture attribute, this function will insert default value to attribute list to fill the memory space.
        // User can use Set*** function to set attribute value
        virtual Int InsertVertex(const Vector3& coord);   
        
        // Return inserted vertex id
        Int InsertVertex(const Vector3& coord, const Vector3& normal);

        virtual void SwapVertex(Int vertexId0, Int vertexId1); 
        virtual void PopbackVertices(Int popCount);
        virtual void SwapTriangles(Int fid0, Int fid1);
        virtual void PopbackTriangles(Int popCount);

        virtual void UpdateNormal(void);     
        virtual void Clear(void);

        // Set** api will allocate memory for the correspondant information
        // Set**(false) will only mark the attribute flag, and don't free memory
        void SetHasVertexColor(bool has);
        bool HasVertexColor(void) const;
        void SetHasVertexTexCoord(bool has);
        bool HasVertexTexCoord(void) const;
        void SetHasTriangleColor(bool has);
        bool HasTriangleColor(void) const;
        void SetHasTriangleTexCoord(bool has);
        bool HasTriangleTexCoord(void) const;

        // This is used in STL mesh structure, since STL format has no topology information.
        // FuseVertex will make vertices as one vertex if their coorindates' distance is small enough
        ErrorCode FuseVertex(void);

        void SetMeshType(MeshType meshType);
        MeshType GetMeshType(void) const;

        Vector3 GetVertexColor(Int vid) const;
        void SetVertexColor(Int vid, const Vector3& color);
        Vector3 GetVertexTexcoord(Int vid) const;
        void SetVertexTexcoord(Int vid, const Vector3& texcoord);
        Vector3 GetTriangleColor(Int fid, Int localVid) const;
        void SetTriangleColor(Int fid, Int localVid, const Vector3& color);
        Vector3 GetTriangleTexcoord(Int fid, Int localVid) const;
        void SetTriangleTexcoord(Int fid, Int localVid, const Vector3& texcoord);
        
        void UnifyCoords(Real bboxSize, Real* scaleValue = NULL, Vector3* objCenterCoord = NULL);
        void UnifyCoords(Real scaleValue, const Vector3& objCenterCoord);

        void SetDefaultColor(Vector3 color);

        PointCloud* ExtractPointCloud(bool hasNormal, bool hasColor);

        virtual ~TriMesh();

    private:
        MeshType mMeshType;
        std::vector<Vector3> mVertexCoordList;
        std::vector<Vector3> mVertexNormalList;
        std::vector<Vector3> mVertexColorList;
        std::vector<Vector3> mVertexTexCoordList;
        std::vector<TriangleInfo*> mTriangleList;
        std::vector<Vector3> mTriangleNormalList;
        std::vector<Vector3> mTriangleColorList;
        std::vector<Vector3> mTriangleTexCoordList;
        bool mHasVertexNormal;
        bool mHasVertexColor;
        bool mHasVertexTexCoord;
        bool mHasTriangleNormal;
        bool mHasTriangleColor;
        bool mHasTriangleTexCoord;
        Vector3 mDefaultColor;
    };



    // mVertexId[0] < mVertexId[1]
    // In manifold sturcture: 1. Edge on boundary: mFaceIds.size() == 1; 2. Other: mFaceIds.size() == 2
    struct BGL_EXPORT EdgeInfo
    {
        EdgeInfo();
        EdgeInfo(Int vertexId0, Int vertexId1);
        void SetVertexIds(Int vertexId0, Int vertexId1);

        Int mVertexId[2];
        std::vector<Int> mFaceIds;
    };

    // store the point on edge informations:
    // if the point is exactly on the mesh vertex, the vertex id is stored in mVertexIdStart, and mVertexIdEnd is -1.
    // if the point is on a mesh edge, the two Ints store the two vertex ids for the edge, and the weight indicate the proportion
    // of the point: Point = startVertexPos * mWeight + endVertexPos * (1 - mWeight)
    struct BGL_EXPORT PointOnEdge
    {
        PointOnEdge();
        PointOnEdge(Int vertexIdStart, Int vertexIdEnd, Real weight);

        Int  mVertexIdStart;
        Int  mVertexIdEnd;
        Real mWeight;
    };

    // mCoord: barycentric coordinate
    // if 2 of the mCoord[fvid] is -1.0, it is a vertex of the triMesh, 
    //  and the vertex is the non-(-1.0) vertex of the face mFaceId.
    // if only 1 of the mCoord[fvid] is -1.0, it is a edge-point of the triMesh, 
    //  and the edge' two end vertices are the two non-(-1.0) vertices of the face mFaceId.
    struct BGL_EXPORT PointOnFace
    {
        PointOnFace();
        PointOnFace(Int faceId, const Vector3& coord);

        void Set(Int faceId, const Vector3& coord);

        Vector3 GetCoord(const ITriMesh* triMesh) const;
        
        void SnapCoord(Real presicion);

        Int mFaceId;
        Vector3 mCoord;
    };

    struct BGL_EXPORT OrderedPointOnFace
    {
        OrderedPointOnFace(const PointOnFace& pof, const ITriMesh* triMesh);

        bool operator < (const OrderedPointOnFace& pof) const;

        PointOnFace mPof;
        const ITriMesh* mTriMesh;
    };
}
