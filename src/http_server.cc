#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

#include "http_server.h"

namespace nerver {

http_conn::http_conn(tcp_conn &conn, std::string const &root)
    : tcp_conn_(conn), web_root_(root)
{
    //  nothing
}

http_conn::http_conn(http_conn &&other)
    : tcp_conn_(other.tcp_conn_),
      buf_(std::move(other.buf_)),
      web_root_(other.web_root_)
{
    //  nothing
}

void http_conn::process_request()
{
    if (not message_end())
        return;

    std::istringstream iss(std::string(buf_.data(), buf_.size()));
    std::string word, uri;

    iss >> word;
    if (word != "GET") {
        send_status_line("501", "Not Implemented");
        tcp_conn_.send("\r\n", 2);
    } else {
        iss >> uri;
        send_file(uri);
    }

    tcp_conn_.die();
}

void http_conn::send_status_line(std::string const &status_code, std::string const &reason_phrase)
{
    std::string status_line = "HTTP/1.0 " + status_code + " " + reason_phrase + "\r\n";
    tcp_conn_.send(status_line.c_str(), status_line.size());
}

void http_conn::send_file(std::string const &file)
{
    auto full_path = web_root_ + file;
    std::ifstream ifs(full_path);
    if (not ifs) {
        std::cerr << file << " not found\n";
        send_status_line("404", "Not Found");
        tcp_conn_.send("\r\n", 2);
        return;
    }

    std::cerr << file << " serviced successfully\n";
    send_status_line("200", "OK");
    tcp_conn_.send("\r\n", 2);

    std::ostringstream buffer;
    buffer << ifs.rdbuf();
    auto data = buffer.str();
    tcp_conn_.send(data.data(), data.size());
}

bool http_conn::message_end() const
{
    if (buf_.size() < 4)
        return false;

    for (auto p = buf_.cbegin(); p + 4 <= buf_.cend(); ++p)
        if (std::string(p, p+4) == "\r\n\r\n")
            return true;
    return false;
}

http_server::http_server(std::string const &root)
    : tcp_serv_("80", 6), web_root_(root + "/")
{
    using namespace std::placeholders;

    tcp_serv_.set_establish_callback(std::bind(&http_server::establish_handler, this, _1));
    tcp_serv_.set_message_callback(std::bind(&http_server::message_handler, this, _1, _2, _3));
    tcp_serv_.set_close_callback(std::bind(&http_server::close_handler, this, _1));
}

void http_server::start()
{
    tcp_serv_.start();
}

void http_server::establish_handler(tcp_conn &conn)
{
    std::cerr << "connection from "
              << conn.peer_addr().to_string()
              << " established\n";
    conn_list_.emplace(&conn, http_conn(conn, web_root_));
}

void http_server::message_handler(tcp_conn &conn, char const *buf, std::size_t buf_len)
{
    //  can not use [], which requies default constructor
    http_conn &http = conn_list_.at(&conn);
    http.buf_.insert(http.buf_.cend(), buf, buf + buf_len);
    http.process_request();
    //  std::cerr << std::string(http.buf_.data(), http.buf_.size());
}

void http_server::close_handler(tcp_conn &conn)
{
    std::cerr << "connection from "
              << conn.peer_addr().to_string()
              << " closed\n";
    conn_list_.erase(&conn);
}

}
