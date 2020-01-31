#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#define COMPLETED "Completed"

// send 1GB of data to the server
#define DATA_BYTES (1024*1024*1024)
// write by this block size
#define BLOCK_BYTES (1024*1)

const char data[BLOCK_BYTES] = "NVIDIA DRIVE AGX self-driving compute platforms are built on NVIDIA Xavier™, the world’s first processor designed for autonomous driving. The auto-grade Xavier system-on-a-chip (SoC) is in production today and architected for safety, incorporating six different types of processors to run redundant and diverse algorithms for AI, sensor processing, mapping and driving. Leveraging Xavier, DRIVE AGX platforms process data from camera, lidar, radar, and ultrasonic sensors to understand the complete 360-degree environment in real-time, localize itself to a map, and plan a safe path forward.";

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
    size_t num_blks = DATA_BYTES / BLOCK_BYTES;
    if (num_blks == 0) {
        num_blks = 1;
    }
    for (int i = 0; i < num_blks; ++i) {
        if (write(sock, data, BLOCK_BYTES) < 0) {
            perror("writing on stream socket");
            break;
        }
    }
    close(sock);
}
