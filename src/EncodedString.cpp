#include <cstring>
#include "EncodedString.h"

EncodedString::EncodedString()
: _size(0)
{
    _str = new char[1];
    _str[0] = (char)0xff;
}

EncodedString::EncodedString(const char* str)
{
    size_t i;

    i = 0;
    while (str[i] != (char)0xff)
        i++;
    _size = i;
    _str = new char[i + 1];
    memcpy(_str, str, i + 1);
}

EncodedString::EncodedString(const EncodedString& other)
: _size(other._size)
{
    _str = new char[_size + 1];
    memcpy(_str, other._str, _size + 1);
}

EncodedString::~EncodedString()
{
    delete[] _str;
}

EncodedString& EncodedString::operator=(const EncodedString& other)
{
    delete[] _str;
    _size = other._size;
    _str = new char[_size + 1];
    memcpy(_str, other._str, _size + 1);

    return *this;
}

const char* EncodedString::str() const
{
    return _str;
}

size_t EncodedString::size() const
{
    return _size;
}
