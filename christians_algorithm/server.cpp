//
// Created by rishabh on 05/11/20.
//
#include <iostream>
#include <chrono>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;
#define SEND_CORRECT_TIME 0

const int PORT = 5400;

int start_server(int PORT, in_addr_t IP)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // struct
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = IP;

    // bind
    bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));

    // accept
    listen(server_socket, 5);

    printf("Listening at port %d\n", PORT);

    return server_socket;
}

time_t get_utc_time(time_t T)
{
    tm *utc_time{};
    // Ts has cur time
    time(&T);

    utc_time = gmtime(&T);

    return mktime(utc_time);
}

void server_client(int client_socket)
{
    int request_code;
    recv(client_socket, &request_code, sizeof(int), 0);
    cout << "Request Code  : " << request_code << endl;
    if (request_code == SEND_CORRECT_TIME)
    {
        // change this to get utc time
        time_t Ts = 0;
        Ts = get_utc_time(Ts);
        cout << Ts << endl;

        // To introduct some round trip time
        sleep(Ts % 3);
        send(client_socket, &Ts, sizeof(Ts), 0);
    }
}

int main()
{
    // bind
    int server_socket = start_server(PORT, INADDR_ANY);
    // recieve connection
    while (true)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        server_client(client_socket);
        close(client_socket);
    }
}