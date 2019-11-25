#include <iostream>

#include <unistd.h>

#include "event_loop.h"
#include "acceptor.h"
#include "exception.h"

using namespace nerver;

void on_new_connection(Socket s, inet_addr peer_addr)
{
    std::cout << "new connection from "
              << peer_addr.to_string()
              << '\n';
    LCHECK_PRINT(write(s.fd(), "hello, world\n", 13));
}

int main()
{

    event_loop loop;
    inet_addr addr("localhost", "9999", true, AF_INET6);

    acceptor a(loop.get_poller(), addr);
    a.set_connection_callback(on_new_connection);
    a.listen();

    loop.loop();
}
