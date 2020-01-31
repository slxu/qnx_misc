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

#define BLOCK_BYTES (1024*4)

const char data[BLOCK_BYTES] = "This program creates a pair of connected sockets, then forks and communicates over them. This is very similar to communication with pipes, however, socketpairs are two-way  communications  objects. Therefore I can send messages in both directions.";

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in receiver;
    struct hostent *hp;

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }
    /* Connect socket using name specified by command line.  */
    receiver.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
        fprintf(stderr, "%s: unknown host\n", argv[1]);
        exit(2);
    }
    memcpy(&receiver.sin_addr, hp->h_addr, hp->h_length);
    receiver.sin_port = htons(atoi(argv[2]));

    size_t num_blks = 1024*1024*1024 / BLOCK_BYTES;
    if (num_blks == 0) {
        num_blks = 1;
    }
    for (int i = 0; i < num_blks; ++i) {
        if (sendto(sock, data, BLOCK_BYTES, 0,
                  (struct sockaddr *)&receiver, sizeof(receiver)) < 0) {
            perror("sending datagram message");
            break;
        }
    }
}
