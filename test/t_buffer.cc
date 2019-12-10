#include <iostream>

#include "buffer.h"

int main()
{
    nerver::buffer buf(4);

    buf.append("ab", 2);
    buf.append("cdef", 4);
    buf.pop(4);
    buf.pop(2);
    buf.append("hello", 5);
}
