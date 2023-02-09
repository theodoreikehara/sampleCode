#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090

// Define the packet structure
struct packet {
    int seq_num;
    char data[100];
};

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t cli_len;
    struct packet pkt;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: Failed to create socket\n");
        exit(1);
    }

    // Initialize the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error: Failed to bind socket\n");
        exit(1);
    }

    while (1) {
        // Receive a packet from the client
        cli_len = sizeof(client_addr);
        if (recvfrom(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *) &client_addr, &cli_len) < 0) {
            perror("Error: Failed to receive packet\n");
            exit(1);
        }

		printf("Received packet %d with data: %s\n", pkt.seq_num, pkt.data);

        // Echo the packet back to the client
        if (sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *) &client_addr, cli_len) < 0) {
            perror("Error: Failed to send packet\n");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}
