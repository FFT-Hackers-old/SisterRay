#ifndef ENCODED_STRING_H
#define ENCODED_STRING_H

#include <cstddef>

class EncodedString
{
public:
    EncodedString();
    EncodedString(const char* str);
    EncodedString(const EncodedString& other);
    ~EncodedString();

    EncodedString& operator=(const EncodedString& other);

    const char* str() const;
    size_t      size() const;

private:
    size_t      _size;
    char*       _str;
};

#endif
