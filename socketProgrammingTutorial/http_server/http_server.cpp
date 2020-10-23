#include <iostream>
#include <string>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;
const int PORT = 4568;
int main()
{
    FILE *file_data;
    file_data = fopen("./index.html", "r");

    // store the data
    char response_data[1024];
    fgets(response_data, 1024, file_data);

    printf("%s", response_data);

    // whenever our client connects .. we send it index.html data
    // header
    char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";

    // merge
    strcat(http_header, response_data); // http header -> data packet

    printf("%s", http_header);

    // server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // create a struct
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    if (bind(server_socket, (sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "Couldn't bind port to socket" << endl;
        exit(EXIT_FAILURE);
    }
    // listen to calls
    if (listen(server_socket, 5) < 0)
    {
        cerr << "Server error " << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Listening to incoming requests on port : " << PORT << endl;

    while (true)
    {
        // cout << PORT << endl;
        // accept connection
        int client_socket = accept(server_socket, NULL, NULL); // problem
        // cout << PORT << endl;

        // cout << PORT << endl;

        // send data
        send(client_socket, http_header, sizeof(http_header), 0);

        // close client_socket
        close(client_socket);
        // cout << PORT << endl;
    }

    close(server_socket);
}

// http://localhost:4568/
// simple http server in c++
// html client