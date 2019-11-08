#ifndef NERVER_EXCEPTION_H
#define NERVER_EXCEPTION_H

#include <string>
#include <exception>

namespace nerver {

class sys_error : public std::exception {
public:
    sys_error(int errnum, std::string const &error_msg);
    char const *what() const noexcept override;
    int error_code() const;
private:
    int errno_;
    std::string msg_;
};

int pcheck_throw(int errnum, char const *file, int line);
int pcheck_print(int errnum, char const *file, int line);
int lcheck_throw(int errnum, char const *file, int line);
int lcheck_print(int errnum, char const *file, int line);

std::string error_msg(int errnum, char const *file, int line);

#define PCHECK_THROW(e)     pcheck_throw((e), __FILE__, __LINE__)
#define PCHECK_PRINT(e)     pcheck_print((e), __FILE__, __LINE__)
#define LCHECK_THROW(e)     lcheck_throw((e), __FILE__, __LINE__)
#define LCHECK_PRINT(e)     lcheck_print((e), __FILE__, __LINE__)

}   // namespace nerver

#endif  // #ifndef NERVER_EXCEPTION_H
