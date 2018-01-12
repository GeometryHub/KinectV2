/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "IPointCloud.h"
#include "IGridPointCloud.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT PointCloud : public IPointCloud, public IGridPointCloud
    {
    public:
        // In the default constructor, PointCloud has no normal, no color information, no grid topology
        // User can set the information in the constructor or use SetHas** api to allocate the memory
        PointCloud();
        PointCloud(bool hasNormal, bool hasColor, bool isGrid = false);

        // IPointCloud Interface
        virtual Int GetPointCount() const;
        virtual Vector3 GetPointCoord(Int pid) const;
        virtual void SetPointCoord(Int pid, const Vector3& coord);
        virtual Vector3 GetPointNormal(Int pid) const;
        virtual void SetPointNormal(Int pid, const Vector3& normal);
        virtual bool HasNormal() const;
        virtual void SetHasNormal(bool has);
        // InsertPoint, Clear will make isGrid = false
        virtual Int InsertPoint(const Vector3& coord);
        virtual Int InsertPoint(const Vector3& coord, const Vector3& normal);
        virtual void SwapPoint(Int pointId0, Int pointId1); 
        virtual void PopbackPoints(Int popCount);
        virtual void Clear(void);

        // PointCloud Functions
        void SetHasColor(bool has);
        bool HasColor(void) const;
        // If pointCloud has no color, GetPointColor will return default color. 
        // User can set default color by SetDefaultColor
        Vector3 GetPointColor(Int pid) const;
        void SetPointColor(Int pid, const Vector3& color);
        void SetDefaultColor(const Vector3& color);

        void ReservePoint(Int pointCount);
        void UnifyCoords(Real bboxSize, Real* scaleValue = NULL, Vector3* objCenterCoord = NULL);
        void UnifyCoords(Real scaleValue, const Vector3& objCenterCoord);

        // IGridPointCloud Interface
        // User can ignore these api if point cloud is not grid
        virtual void InitGrid(Int width, Int height);
        virtual void RemoveOuterBlankGrids(void);
        virtual bool HasGridNormal(void) const;
        virtual void SetHasGridNormal(bool has);
        virtual Int GetWidth(void) const;
        virtual Int GetHeight(void) const;
        virtual bool IsGridValid(Int wid, Int hid) const;
        virtual void InvalidateGrid(Int wid, Int hid);
        virtual Vector3 GetGridCoord(Int wid, Int hid) const;
        virtual void SetGridCoord(Int wid, Int hid, const Vector3& coord);
        virtual Vector3 GetGridNormal(Int wid, Int hid) const;
        virtual void SetGridNormal(Int wid, Int hid, const Vector3& normal);
        virtual Int GetValidGridCount(void) const;
        virtual Vector3 GetValidGridCoord(Int gridId) const;
        virtual Vector3 GetValidGridNormal(Int gridId) const;
        virtual Int GetGridPointId(Int wid, Int hid) const;
        virtual void GetPointGridId(Int pointId, Int& wid, Int& hid) const;
        bool HasGridTopology(void) const;
        void ClearGridTopologyInfo(void);

        virtual ~PointCloud();

    private:
        bool mHasNormal;
        bool mHasColor;
        bool mIsGrid;
        std::vector<Vector3> mCoordList;
        std::vector<Vector3> mNormalList;
        std::vector<Vector3> mColorList;
        std::vector<Int> mGrid2PointMap;
        std::vector<Int> mPoint2GridMap;
        Int mGridWidth;
        Int mGridHeight;
        Vector3 mDefaultColor;
    };
}
