/*==================================================================================================

                       Copyright (c) 2017 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "IPointCloud.h"

namespace GPP
{
    class BGL_EXPORT IGridPointCloud : public IPointCloud
    {
    public:
        IGridPointCloud() {}

        // Initialise all variables
        virtual void InitGrid(Int width, Int height) = 0;
        // Making valid grid bounding box as small as possible
        virtual void RemoveOuterBlankGrids(void) = 0;
        virtual bool HasGridTopology(void) const = 0;

        virtual Int GetWidth(void) const = 0;
        virtual Int GetHeight(void) const = 0;
        
        virtual bool IsGridValid(Int wid, Int hid) const = 0;
        virtual void InvalidateGrid(Int wid, Int hid) = 0;

        virtual Int GetGridPointId(Int wid, Int hid) const = 0;;
        virtual void GetPointGridId(Int pointId, Int& wid, Int& hid) const = 0;
        
        // If grid is valid: it will set grid coord 
        // If grid is invalid: it will make grid valid, and set its coord.
        virtual void SetGridCoord(Int wid, Int hid, const Vector3& coord) = 0;
        virtual Vector3 GetGridCoord(Int wid, Int hid) const = 0;
        
        // Grid should be valid and GridPointCloud should has normal
        virtual void SetGridNormal(Int wid, Int hid, const Vector3& normal) = 0;
        virtual Vector3 GetGridNormal(Int wid, Int hid) const = 0;

        virtual void SetGridColor(Int wid, Int hid, const Vector3& color) = 0;
        virtual Vector3 GetGridColor(Int wid, Int hid) const = 0;
        
        virtual ~IGridPointCloud() {}
    };
}
