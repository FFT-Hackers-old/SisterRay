#ifndef GZIP_H
#define GZIP_H

#include <stdio.h>
#include <zlib.h>
#include <SisterRay/SisterRay.h>

typedef struct {
    u16         deflatedSize; // compressed size
    u16         inflatedSize; // uncompressed size
    z_stream    zstream;
    const u8*   src;
    char*       inBuffer;
    char*       outBuffer;
    char*       outStart;
    u32         cursor;
} SrGzipStream;

SISTERRAY_API void srGzipStreamOpen(SrGzipStream* stream, const u8* src, size_t srcSize, size_t dstSize);
SISTERRAY_API size_t srGzipInflateCpy(const u8* src, size_t srcSize, u8* dst, size_t dstSize);
size_t srGzipRead(SrGzipStream& stream, u8* dst, size_t readSize);
u32 getFileSize(const char* filename);

#endif
