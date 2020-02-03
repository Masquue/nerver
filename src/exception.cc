#include <cerrno>
#include <cstring>
#include <sstream>
#include <iostream>

#include <netdb.h>

#include "exception.h"

namespace nerver {

sys_error::sys_error(int errnum, std::string const &error_msg)
    : errno_(errnum), msg_(error_msg)
{
    // nothing
}

char const *sys_error::what() const noexcept
{
    return msg_.c_str();
}

int sys_error::error_code() const
{
    return errno_;
}

int pcheck_throw(int errnum, char const *file, int line)
{
    if (__builtin_expect(errnum != 0, 0))
        throw sys_error(errnum, error_msg(errnum, file, line, std::strerror));
    return errnum;
}

int pcheck_print(int errnum, char const *file, int line)
{
    if (__builtin_expect(errnum != 0, 0))
        std::cerr << error_msg(errnum, file, line, std::strerror) << std::flush;
    return errnum;
}

int lcheck_throw(int errnum, char const *file, int line)
{
    if (__builtin_expect(errnum == -1, 0))
        throw sys_error(errno, error_msg(errno, file, line, std::strerror));
    return errnum;
}

int lcheck_print(int errnum, char const *file, int line)
{
    if (__builtin_expect(errnum == -1, 0))
        std::cerr << error_msg(errno, file, line, std::strerror) << std::flush;
    return errnum;
}

int gaicheck_throw(int errnum, char const *file, int line)
{
    if (__builtin_expect(errnum != 0, 0))
        throw sys_error(errno, error_msg(errno, file, line, gai_strerror));
    return errnum;
}

std::string error_msg(int errnum, char const *file, int line, strerror_func str_error)
{
    std::ostringstream oss;
    oss << "errnum: " << errnum
        << ", decription: " << str_error(errnum) << '\n'
        << "in file " << file 
        << " at line " << line << '\n';
    return oss.str();
}

}   // namespace nerver
