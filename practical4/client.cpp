#include <iostream>
#include <string>

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5400;

int main()
{

    /* Basic connection code */
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // server object
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // connect to server
    connect(client_socket, (sockaddr *)&server_address, sizeof(server_address));
    // print("response")

    /*          Recieve a responce from server after connecting                 */
    char server_response[2048];

    recv(client_socket, &server_response, sizeof(server_response), 0);

    printf("%s\n", server_response);
    // send string

    /*              Send your name to the server                            */
    char name[1024];
    printf("Enter your name : ");
    fgets(name, sizeof(name), stdin);

    send(client_socket, name, sizeof(name), 0);

    // input name

    /*              Recieve a response from the server                      */
    char final_server_response[2048];
    recv(client_socket, final_server_response, sizeof(final_server_response), 0);

    printf("Server sent a response : \n\t%s\n", final_server_response);

    fflush(stdout);

    close(client_socket);

    return 0;
}