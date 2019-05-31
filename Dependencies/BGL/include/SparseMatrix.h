/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include <vector>
#include <string>

namespace GPP
{
    class BGL_EXPORT Triplet
    {
    public:
        Triplet();
        Triplet(Int row, Int col, Real value);
        ~Triplet();

        Int  GetRow() const;
        void SetRow(Int row);
        Int  GetCol() const;
        void SetCol(Int col);
        Real GetValue() const;
        void SetValue(Real value);
    
    private:
        Int mRow, mCol;
        Real mValue;
    };

    class SparseMatrixImpl;
    class BGL_EXPORT SparseMatrix
    {
    public:
        SparseMatrix();
        SparseMatrix(Int nRow, Int nCol);
        SparseMatrix(Int nRow, Int nCol, const std::vector<Triplet>& tripletList);
        SparseMatrix(const SparseMatrix& matrix);
        ~SparseMatrix();
        
        SparseMatrixImpl* GetImpl(void);
        const SparseMatrixImpl* GetImpl(void) const;

        void Init(Int nRow, Int nCol);
        bool AddTriplet(Int row, Int col, Real value);
        bool BuildFromTriplets(void);
        SparseMatrix Transpose(void) const;
        void PrintMatrix(std::string matrixName) const;

        SparseMatrix& operator = (const SparseMatrix& matrix);

    private:
        SparseMatrixImpl* mpImpl;
    };

    class LinearSparseLUSolverImpl;
    class BGL_EXPORT LinearSparseLUSolver
    {
    public:
        LinearSparseLUSolver();
        ~LinearSparseLUSolver();

        ErrorCode Factorize(const SparseMatrix& sparseMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result);
        void Free(void);

    private:
        LinearSparseLUSolverImpl* mpImpl;
    };

    // sparseMatrix should be SPD: symmetric positive definite
    class LinearSparseLLTSolverImpl;
    class BGL_EXPORT LinearSparseLLTSolver
    {
    public:
        LinearSparseLLTSolver();
        ~LinearSparseLLTSolver();

        ErrorCode Factorize(const SparseMatrix& sparseMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result);
        void Free(void);

    private:
        LinearSparseLLTSolverImpl* mpImpl;
    };

    // sparseMatrix should be SPD: symmetric positive definite
    class LinearSparseCGSolverImpl;
    class BGL_EXPORT LinearSparseCGSolver
    {
    public:
        LinearSparseCGSolver();
        ~LinearSparseCGSolver();

        void SetMaxIteration(Int maxCount);
        void SetConvergentTolerance(Real tol);
        ErrorCode Factorize(const SparseMatrix& sparseMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result, const std::vector<Real>* initGuess = NULL);
        void Free(void);

    private:
        LinearSparseCGSolverImpl* mpImpl;
    };

    class LeastSquareSparseLLTSolverImpl;
    class BGL_EXPORT LeastSquareSparseLLTSolver
    {
    public:
        LeastSquareSparseLLTSolver();
        ~LeastSquareSparseLLTSolver();

        ErrorCode Factorize(const SparseMatrix& sparseMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result);
        void Free(void);

    private:
        LeastSquareSparseLLTSolverImpl* mpImpl;
    };

    class LeastSquareSparseCGSolverImpl;
    class BGL_EXPORT LeastSquareSparseCGSolver
    {
    public:
        LeastSquareSparseCGSolver();
        ~LeastSquareSparseCGSolver();

        void SetMaxIteration(Int maxCount);
        void SetConvergentTolerance(Real tol);
        ErrorCode Factorize(const SparseMatrix& sparseMatrix);
        ErrorCode Solve(const std::vector<Real>& vecB, std::vector<Real>* result, const std::vector<Real>* initGuess = NULL);
        void Free(void);

    private:
        LeastSquareSparseCGSolverImpl* mpImpl;
    };

    class SymGenSparseEigenSolverImpl;
    class BGL_EXPORT SymGenSparseEigenSolver
    {
    public:
        enum SelectType
        {
            ST_LARGEST = 0,
            ST_SMALLEST
        };

    public:
        SymGenSparseEigenSolver();
        ~SymGenSparseEigenSolver();

        // matA: Symmetric Sparse Matrix
        // matB: Symmetric Sparse Matrix, and should be Cholesky decomposition
        ErrorCode Compute(const SparseMatrix& matA, const SparseMatrix& matB, Int k, SelectType st);

        void GetEigenVector(Int index, std::vector<Real>& eigenVector) const;

        Real GetEigenValue(Int index) const;

        // it is used for debug
        void CheckResult(bool check);

    private:
        SymGenSparseEigenSolverImpl* mpImpl;
        bool mCheckResult;
    };
}
