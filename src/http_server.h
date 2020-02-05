#ifndef NERVER_HTTP_SERVER_H
#define NERVER_HTTP_SERVER_H

#include <map>
#include <string>

#include "tcp_server.h"

namespace nerver {

class http_server;

class http_conn {
    friend class http_server;

public:
    http_conn(tcp_conn &conn, std::string const &root);

    //  http_conn is non-copyable
    http_conn(http_conn const &other) = delete;
    http_conn &operator=(http_conn const &other) = delete;

    http_conn(http_conn &&other);

private:
    void process_request();
    void send_status_line(std::string const &status_code, std::string const &reason_phrase);
    void send_file(std::string const &file);
    bool message_end() const;

private:
    tcp_conn &tcp_conn_;
    std::vector<char> buf_;
    std::string web_root_;
};

class http_server {
public:
    http_server(std::string const &root);

    void start();

private:
    void establish_handler(tcp_conn &conn);
    void message_handler(tcp_conn &conn, char const *buf, std::size_t buf_len);
    void close_handler(tcp_conn &conn);

private:
    tcp_server tcp_serv_;
    std::string web_root_;
    std::map<tcp_conn *, http_conn> conn_list_;
};

}

#endif  // #ifndef NERVER_HTTP_SERVER_H
