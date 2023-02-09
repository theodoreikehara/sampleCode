#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUMBER 9090

struct packet {
    int seq_num;
    char data[100];
};

int main(void) {
    int client_socket;
    struct sockaddr_in server_address;

    /* create the socket */
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    /* construct the server address */
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUMBER);
    server_address.sin_addr.s_addr = inet_addr("172.28.122.176");

    /* send the packet to the server */
    struct packet pkt;
    pkt.seq_num = 1;
    strcpy(pkt.data, "This is a test packet and more stuff to test if the server recv");
    if (sendto(client_socket, &pkt, sizeof(pkt), 0, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("sendto() failed");
        exit(EXIT_FAILURE);
    }

    /* receive the response from the server */
    socklen_t server_address_len = sizeof(server_address);
    if (recvfrom(client_socket, &pkt, sizeof(pkt), 0, (struct sockaddr *) &server_address, &server_address_len) < 0) {
        perror("recvfrom() failed");
        exit(EXIT_FAILURE);
    }

    /* print the response from the server */
    printf("Received packet from server: seq_num=%d, data=%s\n", pkt.seq_num, pkt.data);

    /* close the socket */
    close(client_socket);

    return 0;
}
