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

int main()
{
    // bind
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // struct
    sockaddr_in server_address{};
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
        int client_socket = accept(server_socket, nullptr, nullptr);

        int request_code;
        recv(client_socket, &request_code, sizeof(int), 0);
        cout << "Request Code  : " << request_code << endl;
        if(request_code == SEND_CORRECT_TIME){

            // introduce a random delay (bw 0, 100)
            // get time of cur
            time_t Ts = chrono::system_clock::to_time_t(chrono::system_clock::now());
            cout << Ts << endl;
            sleep(Ts % 10);
            send(client_socket, &Ts, sizeof(Ts), 0);

        }

        close(client_socket);
    }

}