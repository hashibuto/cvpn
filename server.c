#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>

int serve(int port) {
    int server_fd;
    struct sockaddr_in addr;
    int opt = 1;
    fd_set set;
    fd_set read_set;
    struct timeval timeout;
    struct sockaddr_in client;
    size_t client_addr_size;
    int max_packet_size = 2048;
    char in_buffer[max_packet_size];

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        close(server_fd);
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        close(server_fd);
        return 1;
    }

    
    FD_ZERO(&set);
    FD_SET(server_fd, &set);


    while (1) {
        read_set = set;
        if (select(FD_SETSIZE, &set, NULL, NULL, NULL) < 0) {
            close(server_fd);
            return 1;
        }

        for (int fd = 0; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &set)) {
                if (fd == server_fd) {
                    // Accept the new connection
                    int client_fd = accept(fd, (struct sockaddr *)&client, &client_addr_size);
                    if (client_fd < 0) {
                        close(client_fd);
                    } else {
                        FD_SET(client_fd, &set);
                    }
                    printf("CONN: arrived from %s on port %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                } else {
                    // Client data is arriving
                    int nbytes = read(fd, &in_buffer, max_packet_size);
                    if (nbytes <= 0) {
                        close(fd);
                        FD_CLR(fd, &set);
                    }
                }
            }
        }
    }

    return 0;
}