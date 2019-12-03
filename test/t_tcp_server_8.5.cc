#include <iostream>

#include <unistd.h>

#include "tcp_server.h"

using namespace nerver;

void on_establishment(tcp_conn &conn)
{
    std::cout << "connection from "
              << conn.peer_addr().to_string()
              << " established\n";
}

void on_close(tcp_conn &conn)
{
    std::cout << "connection from "
              << conn.peer_addr().to_string()
              << " closed\n";
}

void on_message(tcp_conn &conn, char *buf, std::size_t buf_len)
{
    if (buf_len == 0)
        conn.die();
    else 
        write(STDIN_FILENO, buf, buf_len);
}

int main()
{
    tcp_server echo_server("9999", 6);
    echo_server.set_establish_callback(on_establishment);
    echo_server.set_message_callback(on_message);
    echo_server.set_close_callback(on_close);
    echo_server.start();
}
