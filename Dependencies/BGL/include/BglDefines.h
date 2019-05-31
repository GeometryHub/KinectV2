/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include <fstream>
#include <string>

#ifdef BGL_DLL_EXPORT
#ifdef WIN32
#ifdef BGL_BUILD
#define BGL_EXPORT __declspec(dllexport)
#else
#define BGL_EXPORT __declspec(dllimport)
#endif
#else
#define BGL_EXPORT
#endif
#else
#define BGL_EXPORT
#endif

#ifndef NULL
#define NULL 0
#endif

#define GPPFREEPOINTER(pointer) if ((pointer) != NULL) { delete (pointer); (pointer) = NULL; }
#define GPPFREEARRAY(pointer) if ((pointer) != NULL) { delete [](pointer); (pointer) = NULL; }

// Error Codes
#define GPP_NO_ERROR           0
#define GPP_INVALID_INPUT     -2
#define GPP_INVALID_RESULT    -3
#define GPP_NOT_INITIALIZED   -5
#define GPP_API_IS_NOT_AVAILABLE -6
#define GPP_API_IS_NOT_DEFINE -8

// ApiName
#define POINT_TRIANGULATION_CONSTRAINED_DELAUNAY_2D  100000
#define MESH_SPLIT_SPLITBYLINE 100001
#define MESH_SPLIT_SPLITBYPLANE 100002
#define CURVE_SMOOTHCURVEONMESH 100003
#define CURVE_SMOOTHCURVECROSSMESH 100004

#define REALDOUBLE
namespace GPP
{
#ifdef REALDOUBLE
    typedef double Real;
#else
    typedef float Real;
#endif
    typedef short Short;
    typedef unsigned short UShort;
    typedef int Int;
    typedef unsigned int UInt;
    typedef long long LongInt;
    typedef unsigned long long ULongInt;
    typedef int ErrorCode;
    typedef int ApiName;
    extern BGL_EXPORT const Real REAL_TOL;
    extern BGL_EXPORT const Real REAL_LARGE;
    extern BGL_EXPORT const Short SHORT_LARGE;
    extern BGL_EXPORT const UShort USHORT_LARGE;
    extern BGL_EXPORT const Int INT_LARGE;    
    extern BGL_EXPORT const UInt UINT_LARGE;    
    extern BGL_EXPORT const Real GPP_PI;
    extern BGL_EXPORT const Real ONE_RADIAN;
    extern BGL_EXPORT const Real POFPRECISION;
}
