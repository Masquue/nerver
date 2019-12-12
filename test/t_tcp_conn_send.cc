#include <iostream>

#include <cstdlib>

#include "tcp_server.h"

using namespace nerver;

std::size_t repetition_time = 10;

void on_connection(tcp_conn &conn)
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

void on_message(tcp_conn &conn, char const *data, std::size_t len)
{
    if (len == 0) {
        conn.shutdown();
        return;
    }

    std::string msg(data, len), msg_send(msg);
    for (unsigned i = 0; i < repetition_time; ++i)
        msg_send += msg + std::to_string(i);

    conn.send(msg_send.data(), msg_send.size());
}

int main(int argc, char **argv)
{
    if (argc > 1)
        repetition_time = std::atoi(argv[1]);

    tcp_server s("9999", 6);
    s.set_message_callback(on_message);
    s.set_establish_callback(on_connection);
    s.set_close_callback(on_close);

    s.start();
}
