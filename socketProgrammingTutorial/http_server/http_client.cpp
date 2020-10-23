#include <iostream>
#include <string>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    printf("joompla");

    // create connection
    printf("%d", argc);
    // printf("%s", argv[1]);
    char *ip_address = argv[1];
    printf("%s", ip_address);

    // // create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // server address
    sockaddr_in remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(80);
    // giving directly
    inet_aton(ip_address, (in_addr *)remote_address.sin_addr.s_addr);

    // connect
    connect(client_socket, (sockaddr *)&remote_address, sizeof(remote_address));

    // different type of requests
    char request[] = "GET / HTTP/1.1\r\n\r\n";
    char response[4096];

    send(client_socket, request, sizeof(request), 0);
    printf("sent request to the client");
    recv(client_socket, &response, sizeof(response), 0);

    printf("%s\n", response);

    close(client_socket);

    // exchange data
}