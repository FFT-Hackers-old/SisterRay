#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LZSS_BUFFER 4096

void lzssDecompress(char* dst, size_t dstLen, FILE* file)
{
    char buffer[LZSS_BUFFER];
    size_t bufferPos;

    u328_t b;
    u328_t blockControl;
    u328_t ref[2];
    u3232_t fileLen;
    u3232_t fileCursor;
    u3216_t offset;
    u328_t len;
    u3216_t run;
    size_t outLen;

    fread(&fileLen, 4, 1, file);
    fileCursor = 0;
    outLen = 0;
    bufferPos = 0xFEE;

    memset(dst, 0, dstLen);
    memset(buffer, 0, LZSS_BUFFER);

    for (;;)
    {
        fread(&blockControl, 1, 1, file);
        fileCursor++;
        for (u328_t i = 0; i < 8; ++i)
        {
            if (blockControl & (1 << i))
            {
                /* Literal */
                fread(buffer + bufferPos, 1, 1, file);
                dst[outLen++] = buffer[bufferPos];
                bufferPos = (bufferPos + 1) % LZSS_BUFFER;
                fileCursor++;
            }
            else
            {
                /* Compressed */
                fread(ref, 2, 1, file);
                fileCursor += 2;
                len = (ref[1] & 0xf) + 3;
                offset = ref[0] | ((ref[1] & 0xf0) << 4);

                for (u3216_t j = 0; j < len; ++j)
                {
                    b = buffer[offset];
                    dst[outLen++] = b;
                    buffer[bufferPos] = b;
                    offset = (offset + 1) % LZSS_BUFFER;
                    bufferPos = (bufferPos + 1) % LZSS_BUFFER;
                }
            }
        }
        if (fileCursor >= fileLen)
            break;
    }
}
