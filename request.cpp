#include "request.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <vector>

static char *next_token(char *ptr, char separator) {
    while (*ptr && *ptr != separator) {
        ++ptr;
    }
    if (*ptr == '\0') return ptr;

    *ptr = '\0'; // terminate the token by replacing the separator with a \0
    return ptr + 1
        + (separator == '\r') // if \r, then we also need to skip the \n
        + (separator == ':'); // if :, then we also need to skip the following space
}

Request::Request(int sockfd) : buf{0} {
    buf_size = recv(sockfd, buf, sizeof buf, 0);
    if (buf_size < 0) {
        return;
    }
    DEBUG_PRINT(write(STDOUT_FILENO, buf, buf_size));

    char *buf_p = buf;

    // Parse request line
    method = buf_p;
    url = buf_p = next_token(buf_p, ' ');
    version = buf_p = next_token(buf_p, ' ');
    buf_p = next_token(buf_p, '\r');

    // Parse header lines
    while (*buf_p != '\r') {
        char *name = buf_p;
        char *value = buf_p = next_token(buf_p, ':');

        header_names.push_back(name);
        header_values.push_back(value);

        buf_p = next_token(buf_p, '\r');
    }
    body = buf_p + 2; // skip the empty line
}
