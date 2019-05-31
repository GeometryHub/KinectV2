/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"

namespace GPP
{
    class BGL_EXPORT Vector2
    {
    public:
        Vector2();
        Vector2(Real x, Real y);

        void SetValue(Real x, Real y);
        Real Length() const;
        Real LengthSquared() const;
        Real Distance(const Vector2& vec2) const;
        Real DistanceSquared(const Vector2& vec2) const;
        Real Normalise();
        void Rotate(Real angle);
        Real CrossProduct(const Vector2& vec2) const;

        Real     operator [] (int index) const;
        Real&    operator [] (int index);
        bool     operator == (const Vector2& vec2) const;
        bool     operator < (const Vector2& vec2) const; // just used in std::set
        Vector2  operator + (const Vector2& vec2) const;
        Vector2  operator - (const Vector2& vec2) const;
        Vector2  operator * (Real s) const;
        Real     operator * (const Vector2& vec2) const;
        Vector2  operator / (Real s) const;
        Vector2  operator - (void) const;
        Vector2& operator += (const Vector2& vec2);
        Vector2& operator -= (const Vector2& vec2);
        Vector2& operator *= (Real s);
        Vector2& operator /= (Real s);

        ~Vector2();

    private:
        Real mX, mY;
    };
}
