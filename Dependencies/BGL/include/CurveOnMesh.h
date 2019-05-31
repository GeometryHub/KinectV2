/*==================================================================================================

                       Copyright (c) 2017 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "TriMesh.h"
#include "Plane3.h"
#include <vector>

namespace GPP
{
    class TriMeshInfo;
    class IMeshDistance;
    class BGL_EXPORT CurveOnMesh
    {
    public:
        CurveOnMesh();
        ~CurveOnMesh();


        // note: path will through pass mesh vertex, thus it is an approximate geodesics in mesh
        // sectionVertexIds: line segments vertex ids, like vertexId0, vertexId1, vertexId2 ... ... vertexIdn
        // isSectionClose: whether vertexId0 is connected with vertexIdn
        // pathVertexIds: shortest path between sectionVertexIds
        static ErrorCode ComputeApproximateGeodesics(const ITriMesh* triMesh, TriMeshInfo* meshInfo, const std::vector<Int>& sectionVertexIds,
            bool isSectionClose, std::vector<Int>& pathVertexIds, Real& distance, IMeshDistance* meshDistance = NULL);


        // curveList: polyline vertex ids. 
        // smoothWeight: [0, 1]
        // iterationCount >= 0
        // optimizeTopology: erase ear vertex
        // optimizeCurve: if optimizeTopology is true, vertices in curveList may be erased. Thus optimizeCurve stores new curve ids
        static ErrorCode SmoothCurveOnMesh(ITriMesh* triMesh, const std::vector<Int>& curve, 
            Real sharpAngle, Real smoothWeight, Int iterationCount, 
            bool optimizeTopology = false, std::vector<Int>* optimizedCurve = NULL);

        
        static ErrorCode SmoothCurveCrossMesh(const ITriMesh* triMesh, TriMeshInfo* meshInfo, const std::vector<PointOnEdge>& curve,
            Real smoothWeight, Int iterationCount, std::vector<PointOnEdge>& smoothCurve);


        // remove curve intersection
        static ErrorCode ExtractSimpleCurve(const std::vector<Int>& inputCurve, std::vector<Int>& outputCurve);


        static ErrorCode SubdividePolyline(std::vector<GPP::Vector3>& pointList, int subdCount, bool isClose);

    };


    class ITriangleList;
    class BGL_EXPORT IMeshDistance
    {
    public:
        IMeshDistance() {};

        virtual Real GetEdgeLength(Int vertexId0, Int vertexId1) = 0;

        virtual ~IMeshDistance() {};
    };

    class BGL_EXPORT EuclidDistance : public IMeshDistance
    {
    public:
        EuclidDistance(const ITriangleList* triMesh);

        virtual Real GetEdgeLength(Int vertexId0, Int vertexId1);

        virtual ~EuclidDistance();

    private:
        const ITriangleList* mTriMesh;
    };

    class BGL_EXPORT PrincipalCurvatureDistance : public IMeshDistance
    {
    public:
        PrincipalCurvatureDistance(const ITriangleList* triMesh, std::vector<Real>* minCurvature,
            std::vector<Real>* maxCurvature, std::vector<Vector3>* minDirs, std::vector<Vector3>* maxDirs,
            std::vector<bool>* curvatureFlags, bool isMinDir, Real weight);

        virtual Real GetEdgeLength(Int vertexId0, Int vertexId1);

        void SetMinDir(bool isMinDir);

        virtual ~PrincipalCurvatureDistance();

    private:
        void ConstructVertexNeighbors(void);
        void ComputeVertexCurvature(Int vertexId);

    private:
        const ITriangleList* mTriMesh;
        std::vector<Vector3>* mMinDirs;
        std::vector<Vector3>* mMaxDirs;
        std::vector<Real>* mMinCurvature;
        std::vector<Real>* mMaxCurvature;
        std::vector<bool>* mCurvatureFlags;
        bool mIsMinDir;
        Real mWeight;
        std::vector<Vector3> mFaceTensors;
        std::vector<Vector3> mFaceUDirs;
        std::vector<Vector3> mFaceVDirs;
        std::vector<Real> mFaceAreas;
        std::vector<bool> mFaceFlags;
        std::vector<std::vector<Int> > mVertexFaceNeighbors;
    };
}
