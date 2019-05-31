/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include <string>
#include <vector>
#include <Vector3.h>
#include <sstream>

namespace GPP
{
    class PointCloud;
    class IPointCloud;
    class IGridPointCloud;
    class TriMesh;
    class ITriMesh;
    class IQuadMesh;
    class BGL_EXPORT Parser
    {
    public:
        Parser();
        ~Parser();

        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: asc, obj, gbg, gtg, gpc, txt, xyz, ply
        static PointCloud* ImportPointCloud(std::string fileName);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, stl, off, ply, gpt, gmesh
        static TriMesh* ImportTriMesh(std::string fileName);


        // gmesh format
        static TriMesh* ImportTriMeshFromStream(std::stringstream& ss);


        // Support format: gline
        static ErrorCode ImportLineSegments(std::string fileName, std::vector<Vector3>& startCoords, std::vector<Vector3>& endCoords);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, asc, ply, gpc, gbp
        static ErrorCode ExportPointCloud(std::string fileName, const IPointCloud* pointCloud);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: gtg, gbg
        static ErrorCode ExportGridPointCloud(std::string fileName, const IGridPointCloud* pointCloud);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, stl, ply, off, gpt, gmesh
        static ErrorCode ExportTriMesh(std::string fileName, const ITriMesh* triMesh);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj
        static ErrorCode ExportQuadMesh(std::string fileName, const IQuadMesh* quadMesh);


        // Support format: gline
        static ErrorCode ExportLineSegments(std::string fileName, const std::vector<Vector3>& startCoords, const std::vector<Vector3>& endCoords,
            const Int* validPointCount);
    };
}
