#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "response.hpp"
#include "request.hpp"

namespace response {
    static std::string parse_filename(std::string url) {
        // Remove "/"
        std::string filename = url.substr(1);

        // Replace percent-encoded spaces
        size_t pos;
        while ((pos = filename.find("%20")) != std::string::npos) {
            filename.replace(pos, 3, " ");
        }

        // Replace percent-encoded percent signs
        while ((pos = filename.find("%25", pos + 1)) != std::string::npos) {
            filename.replace(pos, 3, "%");
        }

        return filename;
    }

    static std::string parse_filetype(std::string filename) {
        size_t period_pos = filename.find_last_of('.');

        if (period_pos != std::string::npos) {
            return filename.substr(period_pos + 1);
        }
        else {
            return "";
        }
    }

    static std::string create_status_line() {
        // FIXME
        return "HTTP/1.1 200 yabbadabbadoo";
    }

    static std::string get_content_type_header(std::string filetype) {
        const std::map<std::string, std::string> content_type = {
            { "",     "application/octet-stream" },
            { "pdf",  "application/pdf" },
            { "jpg",  "image/jpeg" },
            { "jpeg", "image/jpeg" },
            { "png",  "image/png" },
            { "htm",  "text/html" },
            { "html", "text/html" },
            { "txt",  "text/plain" }
        };

        auto it = content_type.find(filetype);

        if (it != content_type.end()) {
            return "Content-Type: " + it->second + "\r\n";
        } else {
            return "";
        }
    }

    static std::string create_headers(std::string filetype) {
        std::string headers;
        
        headers.append(get_content_type_header(filetype));

        // TODO: Maybe keep-alive or other headers that specify how long browser
        // should hold connection

        return headers;
    }

    static std::vector<uint8_t> create_body(std::string filename) {
        std::vector<uint8_t> content;
        std::ifstream ifs(filename);

        while (ifs.good()) {
            content.push_back(ifs.get());
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
