#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "config.hpp"
#include "request.hpp"

#define TRY(x) if ((x) < 0) { perror(NULL); return errno; }

int main() {
    /* Create socket */
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    TRY(sockfd);

    /* Set options */
    /* The SO_REUSEADDR option allows us to quickly stop/restart the server */
    int opt = 1;
    TRY(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt));

    /* Bind */
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr = {
            .s_addr = INADDR_ANY,
        },
    };
    TRY(bind(sockfd, (struct sockaddr *)&addr, sizeof addr));

    /* Listen */
    TRY(listen(sockfd, BACKLOG));

    /* Accept */
    struct sockaddr_in client_addr; /* this struct is initialized by accept() */
    socklen_t client_addr_len = sizeof client_addr;
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    TRY(client_sockfd);

    /* Read from client and print the request to stdout */
    Request req(client_sockfd);
    TRY(req.buf_size);
    write(STDOUT_FILENO, req.buf, req.buf_size);

    /* Send to client */
    // TODO: Send proper response

    // HTTP/1.1 200 OK\r\n
    // content-type: text/html\r\n
    // \r\n
    // <text data>
    char res[] = "hello, world";
    TRY(send(client_sockfd, res, sizeof res - 1, 0)); /* minus 1 bc of the \0 */

    /* End connection */
    // TODO: Don't shutdown after just one request
    TRY(shutdown(client_sockfd, SHUT_RDWR));
    TRY(close(client_sockfd));

    return 0;
}
