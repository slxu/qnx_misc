#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#define COMPLETED "Completed"

/*
 * This  program creates a  pair of connected sockets,
 * then forks and communicates over them.  This is very
 * similar to communication with pipes, however, socketpairs
 * are  two-way  communications  objects.  Therefore I can
 * send messages in both directions.
 */

const char * data1k = "This program creates a pair of connected sockets, then forks and communicates over them. This is very similar to communication with pipes, however, socketpairs are two-way  communications  objects. Therefore I can send messages in both directions. This program creates a pair of connected sockets, then forks and communicates over them.  This is very similar to communication with pipes, however, socketpairs are  two-way  communications  objects. Therefore I can send messages in both directions. This program creates a pair of connected sockets, then forks and communicates over them.  This is very similar to communication with pipes, however, socketpairs are  two-way  communications  objects. Therefore I can send messages in both directions. This program creates a pair of connected sockets, then forks and communicates over them.  This is very similar to communication with pipes, however, socketpairs are  two-way communications objects. Therefore I can send messages in both directions. This program creates a pair. ";

main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }
    /* Connect socket using name specified by command line.  */
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
        fprintf(stderr, "%s: unknown host\n", argv[1]);
        exit(2);
    }
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr *)&server,
                sizeof(server)) < 0) {
        perror("connecting stream socket");
        exit(1);
    }
    // send 1GB of data to the server
    for (int i = 0; i < 1024* 1024; ++i) {
        if (write(sock, data1k, 1024) < 0) {
            perror("writing on stream socket");
            break;
        }
    }
    close(sock);
}
