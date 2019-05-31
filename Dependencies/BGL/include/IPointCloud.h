/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    class BGL_EXPORT IPointCloud
    {
    public:
        IPointCloud(){}

        virtual Int GetPointCount() const = 0;

        virtual Vector3 GetPointCoord(Int pid) const = 0;
        virtual void SetPointCoord(Int pid, const Vector3& coord) = 0;
        
        virtual bool HasNormal() const = 0;
        virtual void SetHasNormal(bool has) = 0;
        virtual Vector3 GetPointNormal(Int pid) const = 0;
        virtual void SetPointNormal(Int pid, const Vector3& normal) = 0;
        
        virtual bool HasColor(void) const = 0;
        virtual void SetHasColor(bool has) = 0;
        virtual Vector3 GetPointColor(Int pid) const = 0;
        virtual void SetPointColor(Int pid, const Vector3& color) = 0;
        
        // Return inserted point id
        virtual Int InsertPoint(const Vector3& coord) = 0;
        // Return inserted point id
        virtual Int InsertPoint(const Vector3& coord, const Vector3& normal) = 0;
        
        virtual void SwapPoint(Int pointId0, Int pointId1) = 0; 
        virtual void PopbackPoints(Int popCount) = 0;
        virtual void Clear(void) = 0;

        virtual ~IPointCloud(){}
    };
}
