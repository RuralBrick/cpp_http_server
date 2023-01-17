#pragma once

#include <vector>
#include "config.hpp"

struct Request {
    char buf[REQ_BUF_SIZE]; // holds the entire request data
    int buf_size; // size of request

    // These character pointers are null-terminated substrings of buf.
    char *method, *url, *version;
    std::vector<char *> header_names;
    std::vector<char *> header_values;
    char *body;

    Request(int sockfd);
};
