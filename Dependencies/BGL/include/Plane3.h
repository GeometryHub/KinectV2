/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    class BGL_EXPORT Plane3
    {
    public:
        Plane3();
        Plane3(Real parameters[4]);
        Plane3(const Vector3& point0, const Vector3& point1, const Vector3& point2);
        Plane3(const Vector3& point, const Vector3& normal);
        ~Plane3();

        Vector3 GetNormal(void) const; // Vector3(mParameters[0], mParameters[1], mParameters[2])
        Real GetDistance(void) const; // mParameters[3]
        void GetParameters(Real parameters[4]) const;
        void SetGeometry(const Vector3& point, const Vector3& normal);

        Real SignedDistance(const Vector3& point) const;
        
        Vector3 Intersection(const Vector3& startPoint, const Vector3& endPoint, Real* ratio) const;
        
        Vector3 ProjectPoint(const Vector3& point) const;
        Vector3 ProjectVector(const Vector3& vec3) const;

    private:
        Real mParameters[4];
    };
}
