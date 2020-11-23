### This folder contains our c++ implementation of the Christian's algorithm for external synchronisation

In this implementation, we have a *client* and a *server* computer.
The client computer requests the server to send its time and uses Christian's algorithm to correct its clock.

**Commands to run client code**:
1. `g++ -o client client.cpp`
1. `./client`

**Commands to run server code**:
1. `g++ -o server server.cpp`
1. `./server`
