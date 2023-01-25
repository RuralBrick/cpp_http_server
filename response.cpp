#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

#include "response.hpp"
#include "request.hpp"
#include "config.hpp"

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
 
    static std::string create_status_line(int code) {
        const std::map<int, std::string> status_phrases = {
            { 200, "OK" },
            { 400, "Bad Request" },
            { 404, "Not Found" }
        };

        return "HTTP/1.1 "
               + std::to_string(code) + " "
               + status_phrases.at(code) + "\r\n";
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

        return headers + "\r\n";
    }

    static std::vector<uint8_t> create_body(std::string filename) {
        std::vector<uint8_t> content;
        std::ifstream ifs(filename);

        if (ifs.good()) {
            content.assign((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));
        }

        return content;
    }

    static std::vector<uint8_t> build_200_response(
        std::string filename, std::string filetype
    ) {
        std::string status = create_status_line(200);
        std::string headers = create_headers(filetype);
        std::vector<uint8_t> body = create_body(filename);
        
        std::vector<uint8_t> res(status.begin(), status.end());
        res.insert(res.end(), headers.begin(), headers.end());
        res.insert(res.end(), body.begin(), body.end());

        return res;
    }

    std::vector<uint8_t> generate(const Request* req) {
        std::string filename = parse_filename(req->url);
        std::string filetype = parse_filetype(filename);

        std::vector<uint8_t> res;

        if (filename == "") {
            res = build_200_response("index.html", "html");
        }
        else {
            res = build_200_response(filename, filetype);
        }

        DEBUG_PRINT(write(STDOUT_FILENO, res.data(), res.size()));

        return res;
    }
}
