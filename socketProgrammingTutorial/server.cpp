#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define PORT 5400
int main()
{

    char server_response[256] = "Congrats... you have reached the server";

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // create struct
    sockaddr_in server_address;

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));

    // listern
    listen(server_socket, 5);

    // accept a connection
    int client_socket = accept(server_socket, NULL, NULL);

    // send data to client socket

    send(client_socket, &server_response, sizeof(server_response), 0);

    close(server_socket);

    return 0;
    // create a socket
}