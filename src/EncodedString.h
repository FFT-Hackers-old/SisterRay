#ifndef ENCODED_STRING_H
#define ENCODED_STRING_H

#include <cstddef>

class EncodedString {
public:
    EncodedString();
    EncodedString(const char* str);
    EncodedString(const EncodedString& other);
    ~EncodedString();

    EncodedString& operator=(const EncodedString& other);

    const char* str() const;
    const char* unicode() const;
    size_t      size() const;

    static EncodedString from_unicode(const char* str);

private:
    size_t      _size;
    char*       _str;
};

#endif
