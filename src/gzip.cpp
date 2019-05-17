#include "gzip.h"
#include <memory>
#define CHUNK 4096

SISTERRAY_API void srGzipStreamOpen(SrGzipStream* stream, const u8* src, size_t srcSize, size_t dstSize) {
    memset(stream, 0, sizeof(*stream));
    stream->inBuffer = (char*)malloc(CHUNK);
    stream->outBuffer = (char*)malloc(CHUNK);
    stream->deflatedSize = srcSize;
    stream->inflatedSize = dstSize;
    stream->zstream.zalloc = Z_NULL;
    stream->zstream.zfree = Z_NULL;
    stream->zstream.opaque = Z_NULL;
    stream->zstream.avail_in = CHUNK;
    stream->src = src;
    if (stream->zstream.avail_in > stream->deflatedSize)
        stream->zstream.avail_in = stream->deflatedSize;
    stream->zstream.next_in = (Bytef *)stream->inBuffer;
    stream->cursor = stream->zstream.avail_in;
    memcpy(stream->inBuffer, src, stream->zstream.avail_in); //read in the first section of the file for decompression according to avail-in
    stream->zstream.avail_out = CHUNK;
    stream->zstream.next_out = (Bytef *)stream->outBuffer;
    stream->outStart = stream->outBuffer;
    inflateInit2(&stream->zstream, 31);
}

/*decompress gzipped data in buffer src with size srcSize into buffer dst according to readSize; dst is size of extracted buffer*/
SISTERRAY_API size_t srGzipInflateCpy(const u8* src, size_t srcSize, u8* dst, size_t dstSize) {
    /*Initialize the stream object used for decompression*/
    SrGzipStream stream = SrGzipStream();
    srGzipStreamOpen(&stream, src, srcSize, dstSize);
    auto readLength = srGzipRead(stream, dst, dstSize);
    return readLength;
}

size_t srGzipRead(SrGzipStream& stream, u8* dst, size_t readSize) {
    size_t readLength;
    size_t avail;
    size_t remain;

    readLength = 0;
    remain = readSize;

    if (readSize == 0)
        return 0;

    for (;;) {
        /* If data is present in the output buffer, use it */
        if (stream.zstream.avail_out < CHUNK) {
            avail = CHUNK - stream.zstream.avail_out;
            if (remain < avail)
                avail = remain;
            memcpy(dst + readLength, stream.outStart, avail);
            readLength += avail;
            remain -= avail;
            stream.outStart += avail;
            stream.zstream.avail_out += avail;

            if (remain == 0)
                break;
        }

        /* Output buffer is empty, reset it */
        stream.outStart = stream.outBuffer;
        stream.zstream.next_out = (Bytef*)stream.outBuffer;
        stream.zstream.avail_out = CHUNK;

        if (stream.zstream.avail_in == 0) {
            /* Input buffer is empty, refill it */
            avail = CHUNK;
            if (stream.cursor + avail > stream.deflatedSize)
                avail = stream.deflatedSize - stream.cursor;

            /* If all buffers are empty, then we reached EOF */
            if (avail == 0)
                break;

            memcpy(stream.inBuffer, stream.src, avail);
            stream.zstream.avail_in = avail;
            stream.zstream.next_in = (Bytef*)stream.inBuffer;
            stream.cursor += avail;
        }

        inflate(&stream.zstream, Z_NO_FLUSH);
    }
    return readLength;
}
