#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

/*Do not generate copy constructors, throw compile time error*/
class NonCopyable
{
public:
    NonCopyable() {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif
