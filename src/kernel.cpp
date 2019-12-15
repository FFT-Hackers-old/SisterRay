#include <string.h>
#include <stdlib.h>
#include "impl.h"

#define CHUNK 4096

SISTERRAY_API void srKernelStreamOpen(SrKernelStream* stream, FILE* file) {
    memset(stream, 0, sizeof(*stream));
    stream->inBuffer = (char*)malloc(CHUNK);
    stream->outBuffer = (char*)malloc(CHUNK);
    fread(&stream->deflatedSize, 2, 1, file);
    fread(&stream->inflatedSize, 2, 1, file);
    fread(&stream->type, 2, 1, file);
    stream->file = file;
    stream->zstream.zalloc = Z_NULL;
    stream->zstream.zfree = Z_NULL;
    stream->zstream.opaque = Z_NULL;
    stream->zstream.avail_in = CHUNK;
    if (stream->zstream.avail_in > stream->deflatedSize)
        stream->zstream.avail_in = stream->deflatedSize;
    stream->zstream.next_in = (Bytef *)stream->inBuffer;
    stream->cursor = stream->zstream.avail_in;
    fread(stream->inBuffer, stream->zstream.avail_in, 1, file);
    stream->zstream.avail_out = CHUNK;
    stream->zstream.next_out = (Bytef *)stream->outBuffer;
    stream->outStart = stream->outBuffer;
    inflateInit2(&stream->zstream, 31);
}

SISTERRAY_API void srKernelStreamClose(SrKernelStream* stream) {
    inflateEnd(&stream->zstream);
    free(stream->inBuffer);
    free(stream->outBuffer);
}

SISTERRAY_API void srKernelStreamSkip(SrKernelStream* stream) {
    fseek(stream->file, stream->deflatedSize - stream->cursor, SEEK_CUR);
}

SISTERRAY_API size_t srKernelStreamRead(SrKernelStream* stream, void* dst, size_t readSize) {
    char* buf;
    size_t readLength;
    size_t avail;
    size_t remain;

    buf = (char*)dst;
    readLength = 0;
    remain = readSize;

    if (readSize == 0)
        return 0;

    for (;;)
    {
        /* If data is present in the output buffer, use it */
        if (stream->zstream.avail_out < CHUNK)
        {
            avail = CHUNK - stream->zstream.avail_out;
            if (remain < avail)
                avail = remain;
            memcpy(buf + readLength, stream->outStart, avail);
            readLength += avail;
            remain -= avail;
            stream->outStart += avail;
            stream->zstream.avail_out += avail;

            if (remain == 0)
                break;
        }

        /* Output buffer is empty, reset it */
        stream->outStart = stream->outBuffer;
        stream->zstream.next_out = (Bytef*)stream->outBuffer;
        stream->zstream.avail_out = CHUNK;

        if (stream->zstream.avail_in == 0)
        {
            /* Input buffer is empty, refill it */
            avail = CHUNK;
            if (stream->cursor + avail > stream->deflatedSize)
                avail = stream->deflatedSize - stream->cursor;

            /* If all buffers are empty, then we reached EOF */
            if (avail == 0)
                break;

            avail = fread(stream->inBuffer, avail, 1, stream->file);
            stream->zstream.avail_in = avail;
            stream->zstream.next_in = (Bytef*)stream->inBuffer;
            stream->cursor += avail;
        }
        inflate(&stream->zstream, Z_NO_FLUSH);
    }
    return readLength;
}
