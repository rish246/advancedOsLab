#include <iostream>
#include <string>
#include <fstream>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5004;

string get_file_content(char *fileName)
{
    // open file
    string file_content;

    ifstream fin(fileName);

    if (!fin.is_open())
    {
        cerr << "Couldn't open the given file" << endl;
        return "";
    }
    cout << "File is open" << endl;

    // int idx = 0;
    while (!fin.eof())
    {
        string word;
        fin >> word;
        file_content += word;
    }

    return file_content;

    // start reading file

    // return len
}

int main()
{

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // addr
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));

    // listen
    listen(server_socket, 5);

    // accept
    int client_socket = accept(server_socket, NULL, NULL);

    // recieve data

    char fileName[1024];
    recv(client_socket, &fileName, sizeof(fileName), 0);

    // int len_of_file = calculate_length(fileName);
    string file_content = get_file_content(fileName);

    int send_data;
    if (file_content == "")
    {
        send_data = -1;
    }
    else
    {
        send_data = file_content.size();
    }

    // int ?send_data;

    // send data to client in little chunks

    send(client_socket, &send_data, sizeof(int), 0);

    printf("%s\n", fileName);

    close(client_socket);

    close(server_socket);
}