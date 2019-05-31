/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"
#include <vector>

namespace GPP
{
    class GeneralMatrixImpl;
    class BGL_EXPORT GeneralMatrix
    {
    public:
        GeneralMatrix();
        GeneralMatrix(Int nRow, Int nCol); // init value is 0
        GeneralMatrix(Int nRow, Int nCol, const Real* values); //row first
        ~GeneralMatrix();

        GeneralMatrixImpl* GetImpl(void);
        const GeneralMatrixImpl* GetImpl(void) const;

        void Reset(Int nRow, Int nCol); // init value is 0;
        Int GetRowCount() const;
        Int GetColCount() const;
        Real GetValue(Int row, Int col) const;
        void SetValue(Int row, Int col, Real value);

    private:
        GeneralMatrixImpl* mpImpl;
    };

    class GeneralMatrix3Impl;
    class BGL_EXPORT GeneralMatrix3
    {
    public:
        GeneralMatrix3();
        ~GeneralMatrix3();

        GeneralMatrix3Impl* GetImpl(void);
        const GeneralMatrix3Impl* GetImpl(void) const;

        Real GetValue(Int row, Int col) const;
        void SetValue(Int row, Int col, Real value);

        // mat += weight * vec * vecT
        void AddWeightMatrix(Real weight, const Vector3& vec);
        
        bool GetInverseMatrix(GeneralMatrix3& invertMatrix) const;

        void Print(void) const;

    private:
        GeneralMatrix3Impl* mpImpl;
    };

    class LinearGeneralLUSolverImpl;
    class BGL_EXPORT LinearGeneralLUSolver
    {
    public:
        LinearGeneralLUSolver();
        ~LinearGeneralLUSolver();

        ErrorCode Factorize(const GeneralMatrix& generalMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result);
        void Free(void);

    private:
        LinearGeneralLUSolverImpl* mpImpl;
    };

    class LeastSquareGeneralLDLSolverImpl;
    class BGL_EXPORT LeastSquareGeneralLDLSolver
    {
    public:
        LeastSquareGeneralLDLSolver();
        ~LeastSquareGeneralLDLSolver();

        ErrorCode Factorize(const GeneralMatrix& generalMatrix);
        ErrorCode Solve(const GeneralMatrix& generalMatrix, const std::vector<Real>& vecB, std::vector<Real>* result);
        void Free(void);

    private:
        LeastSquareGeneralLDLSolverImpl* mpImpl;
    };

    class SelfAdjointEigenSolverImpl;
    class BGL_EXPORT SelfAdjointEigenSolver
    {
    public:
        SelfAdjointEigenSolver();
        ~SelfAdjointEigenSolver();

        ErrorCode Compute(const GeneralMatrix& generalMatrix);

        ErrorCode Compute(const GeneralMatrix3& generalMatrix);

        // if input is M, it will compute MTM
        ErrorCode ComputeHalf(const GeneralMatrix& generalMatrix);
        
        void GetEigenVector(Int index, std::vector<Real>& eigenVector) const;

        //The eigenvalues are sorted in increasing order
        Real GetEigenValue(Int index) const;

    private:
        SelfAdjointEigenSolverImpl* mpImpl;
    };

    // mat = USV' , svd decomposition
    // res = UV'
    extern BGL_EXPORT void OrthgonalApproximation(const std::vector<Real>& matrixValue, Int nRow, Int nCol, std::vector<Real>& res);

    // mat = USV', svd decomposition
    // res = V'U, and det(res) = 1
    extern BGL_EXPORT void OrthgonalApproximationRT(const std::vector<Real>& matrixValue, Int nRow, Int nCol, std::vector<Real>& res);
}
