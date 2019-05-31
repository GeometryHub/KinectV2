/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"

namespace GPP
{
    class BGL_EXPORT Vector3
    {
    public:
        Vector3();
        Vector3(Real x, Real y, Real z);

        void SetValue(Real x, Real y, Real z);
        Real Length() const;
        Real LengthSquared() const;
        Real Distance(const Vector3& vec3) const;
        Real DistanceSquared(const Vector3& vec3) const;
        Real Normalise();
        Vector3 CrossProduct(const Vector3& vec3) const;
        Real MaxAbs(void) const;

        Real   operator [] (int index) const;
        Real&  operator [] (int index);
        bool     operator == (const Vector3& vec3) const;
        bool     operator < (const Vector3& vec3) const; // just used in std::set
        Vector3  operator + (const Vector3& vec3) const;
        Vector3  operator - (const Vector3& vec3) const;
        Vector3  operator * (Real s) const;
        Real   operator * (const Vector3& vec3) const;
        Vector3  operator / (Real s) const;
        Vector3  operator - (void) const;
        Vector3& operator += (const Vector3& vec3);
        Vector3& operator -= (const Vector3& vec3);
        Vector3& operator *= (Real s);
        Vector3& operator /= (Real s);

        ~Vector3();

    private:
        Real mX, mY, mZ;
    };

    extern BGL_EXPORT Vector3 GetVerticalVector(const Vector3& dir);
}
