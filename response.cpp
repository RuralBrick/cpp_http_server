#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "response.hpp"
#include "request.hpp"

namespace response {
    static std::string parse_filename(std::string url) {
        return url.substr(1);
    }

    static std::string parse_filetype(std::string filename) {
        // TODO
        return "";
    }

    static std::string create_status_line() {
        // FIXME
        return "HTTP/1.1 200 yabbadabbadoo";
    }

    static std::string create_headers(std::string filetype) {
        // TODO
        return "";
    }

    static std::vector<uint8_t> create_body(std::string filename) {
        std::vector<uint8_t> content;

        if (filename == "test.html") {
            // TODO
        }
        else {
            std::string msg = "hello, world";
            content.insert(content.end(), msg.begin(), msg.end());
        }

        return content;
    }

    std::vector<uint8_t> generate(const Request* req) {
        std::cout << req->buf_size << "\n";
        std::cout << req->method << "\n";

        std::cout << req->url << "\n";
        /* %20 -> space
        * %25 -> %
        */

        std::cout << req->version << "\n";
        std::cout << req->body << "\n";

        // TODO: Send proper response

        // HTTP/1.1 200 OK\r\n
        // content-type: text/html\r\n
        // \r\n
        // <text data>

        std::string filename = parse_filename(req->url);
        std::string filetype = parse_filetype(filename);

        std::string preamble = create_status_line() + "\r\n"
                               + create_headers(filetype) + "\r\n";
        std::vector<uint8_t> body = create_body(filename);

        std::vector<uint8_t> res(preamble.begin(), preamble.end());
        res.insert(res.end(), body.begin(), body.end());
        return res;
    }
}
