#ifndef POLYGONS_H
#define POLYGONS_H

#include "types.h"

typedef struct {
    u16 vertex1;
    u16 vertex2;
} PolygonEdge;

typedef struct {
    u16 field_0;
    u16 vertex1;
    u16 vertex2;
    u16 vertex3;
    u16 normals[3];
    u16 edges[3];
    u32 field_14;
} Polygon;

typedef struct {
    u32 field_0;
    u32 verticesCount;
    void *driverData;
    u32 field_C;
    u32 normalIndexes;
    u32 vertices;
    u32 vertexColors;
    u32 textureCoords;
    u32 textureSet;
} PolygonGroup;

typedef struct {
    u32 polygonType;
    u32 polygonGroupBaseOffset;
    u32 polygonGroupLength;
    u32 vertexGroupBaseOffset;
    u32 vertexGroupLength;
    u32 edgeGroupBaseOffset;
    u32 edgeGroupLength;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 TextureCoordsBaseOffset;
    u32 isTextured;
    u32 textureID;
} PGroup;

typedef struct {
    u32 field_0;
    float max_x;
    float max_y;
    float max_z;
    float min_x;
    float min_y;
    float min_z;
} BoundingBox;
#endif // !POLYGON_H
