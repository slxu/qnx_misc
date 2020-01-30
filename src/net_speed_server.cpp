#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define BILLION  1000000000L;

int report_stats(uint64_t bytes, struct timespec& start) {
    struct timespec stop;
    if (clock_gettime(CLOCK_MONOTONIC, &stop) != 0) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }
    auto sec = ( stop.tv_sec - start.tv_sec )
         + (double)( stop.tv_nsec - start.tv_nsec )
           / (double)BILLION;

    printf("transfer speed: %.2f MB/s\n", (bytes/1024.0/1024.0) / sec);
    start = stop;
    return 0;
}

int main()
{
    /* Create socket */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }
    /* Name socket using wildcards */
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 0;
    if (bind(sock, (struct sockaddr *)&server, 
            sizeof(server))) {
        perror("binding stream socket");
        exit(1);
    }
    /* Find out assigned port number and print it out */
    unsigned int length = sizeof(server);
    if (getsockname(sock, (struct sockaddr *)&server, 
            &length)) {
        perror("getting socket name");
        exit(1);
    }
    printf("Socket has port #%d\n", ntohs(server.sin_port));

    /* Start accepting connections */
    listen(sock, 5);
    char buf[1024];
    do {
        int msgsock = accept(sock, 0, 0);
        if (msgsock == -1) {
            perror("accept");
            return EXIT_FAILURE;
        } else {
            struct timespec start, start_step;
            uint64_t bytes = 0;
            const uint64_t report_step = 1024 * 1024; // report speed every 1MB;
            uint64_t next_report_bytes = report_step;
            uint64_t last_report_bytes = 0;
            if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
                perror("clock_gettime");
                return EXIT_FAILURE;
            }
            start_step = start;
            while (true) {
                int rval = read(msgsock, buf,  1024);
                if (rval > 0) {
                    bytes += rval;
                    if (bytes >= next_report_bytes) {
                        report_stats(bytes - last_report_bytes, start_step);
                        last_report_bytes = bytes;
                        next_report_bytes += report_step;
                    }
                    // printf("-->%s\n", buf);
                } else if (rval == 0) {
                    report_stats(bytes, start);
                    printf("Ending connection.\n");
                    break;
                } else {
                    perror("Reading stream message");
                    break;
                }
            }
        }
        close(msgsock);
    } while (TRUE);
}
