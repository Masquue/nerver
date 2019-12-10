#ifndef NERVER_BUFFER_H
#define NERVER_BUFFER_H

#include <vector>

namespace nerver {

class buffer {
public:
    using size_type = std::vector<char>::size_type;

    buffer(size_type init_size = Default_size);

    void append(void const *p, size_type len);
    void pop(size_type n);

    void const *data() const;
    //  number of readable bytes
    size_type size() const;
    bool empty() const;

private:
    size_type available() const;

private:
    std::vector<char> buf_;
    size_type start_, end_;
    //  0 <= start_ <= end_ <= buf_.size()
    //  [0, start_)         : wasted leading memory
    //  [start_, end_)      : data
    //  [end_, buf_.size()) : available space for upcoming data

    static const size_type Default_size;
};

}   // namespace nerver

#endif  // #ifndef NERVER_BUFFER_H
