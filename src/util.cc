#include <csignal>

#include "util.h"

namespace nerver {

void ignore_sigpipe()
{
    signal(SIGPIPE, SIG_IGN);
}

}   // namespace nerver
