#ifndef KERNEL_H
#define KERNEL_H

#include <stdio.h>
#include <zlib.h>
#include <SisterRay/SisterRay.h>

/*TODO refactor this stuff to use the new gzip.h api*/

typedef struct {
    u16         deflatedSize; // compressed size
    u16         inflatedSize; // uncompressed size
    u16         type;
    FILE*       file;
    z_stream    zstream;
    char*       inBuffer;
    char*       outBuffer;
    char*       outStart;
    u32         cursor;
} SrKernelStream;

SISTERRAY_API void   srKernelStreamOpen(SrKernelStream* stream, FILE* file);
SISTERRAY_API void   srKernelStreamClose(SrKernelStream* stream);
SISTERRAY_API void   srKernelStreamSkip(SrKernelStream* stream);
SISTERRAY_API size_t srKernelStreamRead(SrKernelStream* stream, void* dst, size_t size);

typedef void (*SrKernelStreamHandler)(SrKernelStream*);

#endif
