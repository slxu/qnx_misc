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
    int sock;

    /* Create socket from which to read. */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("opening datagram socket");
        exit(1);
    }
    /* Create name with wildcards. */
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = 0;
    if (bind(sock, (struct sockaddr *)&name, sizeof(name))) {
        perror("binding datagram socket");
        exit(1);
    }
    /* Find assigned port value and print it out. */
    unsigned int length = sizeof(name);
    if (getsockname(sock, (struct sockaddr *)&name, &length)) {
        perror("getting socket name");
        exit(1);
    }
    printf("Socket has port #%d\n", ntohs(name.sin_port));    /* Start accepting connections */

    char buf[1024];
    do {
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
            int rval = read(sock, buf,  1024);
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
    } while (TRUE);
}
