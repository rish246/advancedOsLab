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
    // calculate time now
    time_t T0 = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << "Original time of the client clock : " << ctime(&T0) << endl;

    // initialize socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address{};
    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    // connect
    if(connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == -1){
        cerr << "Couldn't connect to server" << endl;
    }

    // send empty message
    int message_code = SEND_CORRECT_TIME;

    // send this message code to server
    send(client_socket, &message_code, sizeof(message_code), 0);

    // recv Ts timestamp from server
    int Ts;
    recv(client_socket, &Ts, sizeof(Ts), 0);
    cout << "Delay occured = " << Ts << endl;
    // compute T1
    auto T1 = chrono::system_clock::to_time_t(chrono::system_clock::now());
    // calculate Tp
    auto round_trip_time = (T1 - T0);

    time_t Tp = ((double) round_trip_time / 2) + Ts;

    cout << "Round trip time of the message :\t" << round_trip_time << endl;
    cout << "Correct time of the clock :\t" << ctime(&Tp) << endl;

    fflush(stdout);
    close(client_socket);


}