#include <iostream>
#include <string>

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5400;

void serve_client(int client_socket)
{
    /*             Recieving initial response from the client               */
    ///////////////////////////////////////////////////////
    char server_response[] = "Congrats.. You've reached the server";

    // send server response to the client
    send(client_socket, server_response, sizeof(server_response), 0);

    // recieve a string
    char client_response[2048];
    recv(client_socket, &client_response, sizeof(client_response), 0);

    printf("Client sent a message : %s\n", client_response);
    ////////////////////////////////////////////////////////////////

    /*             Sending a response back to the client               */

    ///////////////////////////////////////////////////////////////////
    char append_server_response[] = "... You have reached the server";

    int final_response_size = strlen(client_response) + strlen(server_response) + 1;
    char *final_server_response = (char *)malloc(final_response_size); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(final_server_response, client_response);
    strcat(final_server_response, append_server_response);

    send(client_socket, final_server_response, final_response_size, 0);

    printf("Final repsonse to the client : \n\t%s\n", final_server_response);
    //////////////////////////////////////////////////////////////////////////

    printf("One more client served\n\n");
}

int main()
{

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // struct
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));

    // accept
    listen(server_socket, 5);

    printf("Listening at port %d\n", PORT);
    // recieve connection
    while (true)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        serve_client(client_socket);
        close(client_socket);
    }

    return 0;
}