#include "http_server.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;

    nerver::http_server server(argv[1]);
    server.start();
}
