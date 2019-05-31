/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include "Vector3.h"

namespace GPP
{
    // v0  v1  v2  v3
    // v4  v5  v6  v7
    // v8  v9  v10 v11
    // v12 v13 v14 v15
    class BGL_EXPORT Matrix4x4
    {
    public:
        Matrix4x4();
        Matrix4x4(Real initValue);
        Matrix4x4(const Matrix4x4& matrix);
        ~Matrix4x4();

        void Init(Real value);
        void InitIdentityTransform(void);

        Real GetValue(Int index) const;
        void SetValue(Int index, Real value);
        Real GetValue(Int rid, Int cid) const;
        void SetValue(Int rid, Int cid, Real value);

        Vector3 TransformPoint(const Vector3& point) const;
        Vector3 RotateVector(const Vector3& vec) const;
        Vector3 GetTranslation(void) const;
        
        // result = [vec 1] Mat [vec 1]'
        Real QuadricValue(const Vector3& vec) const;
        
        void Free(void);
        bool IsEmpty(void) const;

        Matrix4x4  operator + (const Matrix4x4& mat) const;
        Matrix4x4  operator - (const Matrix4x4& mat) const;
        Matrix4x4  operator * (const Matrix4x4& mat) const;
        Matrix4x4  operator * (Real value) const;
        Matrix4x4& operator += (const Matrix4x4& mat);
        Matrix4x4& operator -= (const Matrix4x4& mat);
        Matrix4x4& operator *= (const Matrix4x4& mat);
        Matrix4x4& operator *= (Real value);
        Matrix4x4& operator = (const Matrix4x4& mat);

        void GenerateScale(const Vector3& scale);
        void GenerateTranslation(const Vector3& translate);
        //source and target are direction: unit vector3
        void GenerateVectorToVectorRotation(const Vector3& source, const Vector3& target);
        // Rotate Shape L from source to target, angle of shape L is 90 degree
        void GenerateLShapeRotation(const Vector3& sourceCenter, const Vector3& sourceTop, const Vector3& sourceRight,
                                    const Vector3& targetCenter, const Vector3& targetTop, const Vector3& targetRight);
        void GenerateUnitQuaternionRotation(Real qx, Real qy, Real qz, Real qw);
       
        Matrix4x4 AffineInverse(void) const;

        void Print(void) const;

    private:
        Real* mpValues;
    };
}
