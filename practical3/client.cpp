#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5004;

int main()
{
    char fileName[] = "./file.txt";

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // server address
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // connect
    connect(client_socket, (sockaddr *)&server_address, sizeof(server_address));

    // send fileName
    send(client_socket, fileName, sizeof(fileName), 0);

    // recv length of file
    int length_of_file;

    recv(client_socket, &length_of_file, sizeof(length_of_file), 0);

    printf("%d\n", length_of_file);

    close(client_socket);

    return 0;
}