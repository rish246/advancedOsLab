#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5400;

int main()
{
    // create connection socket
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // create a socketConfig struct
    sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // ready to connect
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection_status == -1)
    {
        cerr << "Error while connecting socket to config" << endl;
        exit(EXIT_FAILURE);
    }

    // get server response
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    for (char letter : server_response)
        cout << letter;

    close(network_socket);
}