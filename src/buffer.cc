#include <algorithm>

#include "buffer.h"

namespace nerver {

const buffer::size_type buffer::Default_size = 4096;

buffer::buffer(size_type init_size)
    : buf_(init_size), start_(0), end_(0)
{ 
    // nothing
}

void buffer::append(void const *p, size_type len)
{
    auto cp = static_cast<char const *>(p);

    auto num_copy = std::min(len, available());
    std::copy(cp, cp + num_copy, buf_.data() + end_);

    if (num_copy != len)
        buf_.insert(buf_.cend(), cp + num_copy, cp + len);

    end_ += len;
}

void buffer::pop(size_type n)
{
    // it's OK for n to be greater than size()
    if ((start_ += n) >= end_)
        start_ = end_ = 0;
}

void const *buffer::data() const
{
    return buf_.data() + start_;
}

//  number of readable bytes
buffer::size_type buffer::size() const
{
    return end_ - start_;
}

bool buffer::empty() const
{
    return start_ == end_;
}

buffer::size_type buffer::available() const
{
    return buf_.size() - end_;
}

}   // namespace nerver
