#include <string>
#include <fstream>
#include <iostream>
#include <string.h>

#include <math.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define PORT 5400

/*
    @name   --> filesize
    @params --> string filename
    @return --> size of the file having name filename
    @desc   --> gets the size of the file and returns the size of the file
*/
ifstream::pos_type filesize(const char *filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

/*
    @name   --> get_length
    @params --> string filename
    @return --> length of filename
    @desc   --> uses filesize() func to get the size of the file and return to the user
*/
int get_length(const char *filename)
{

    ifstream fin(filename);

    if (!fin.is_open())
    {
        return -1;
    }

    return filesize(filename);
}

/*
    @name   --> send_blocks
    @params --> int client_socket, string filename, int filesize
    @return --> void
    @desc   --> breaks the file into blocks and sends them to the server
*/
void send_blocks(int client_socket, string content, int filesize, int num_blocks)
{
    int max_block_size = 1024;
    for (int i = 0; i < num_blocks; i++)
    {
        // this code has bug
        // fix this code

        int start_ptr = i * max_block_size;
        int end_ptr = min((i + 1) * max_block_size, filesize);

        int cur_block_size = (end_ptr - start_ptr);

        // create a buffer to send
        char buffer[cur_block_size];

        // copy file content to buffer.
        for (int i = start_ptr; i < end_ptr; i++)
        {
            buffer[i - start_ptr] = content[i];
        }
        // send buffer
        send(client_socket, &buffer, sizeof(buffer), 0);

        // cout << "Block " << i << " sent to the client" << endl;
    }
}

/*
    @name   --> send_file
    @params --> int client_socket, string filename, int filesize
    @return --> void
    @desc   --> breaks the file into blocks and sends each block to the client connected via client socket
*/
void send_file(int client_socket, const char *filename, int filesize)
{
    int max_block_size = 1024;

    int num_blocks = ceil(1.0 * filesize / max_block_size);

    bool is_recieved = false;

    // send num_blocks to client
    while (!is_recieved)
    {

        send(client_socket, &num_blocks, sizeof(num_blocks), 0);

        // recv ack
        recv(client_socket, &is_recieved, sizeof(is_recieved), 0);
    }
    cout << "---------------------------------------" << endl;

    printf("Acknowledgement recieved... sending file..\n");
    cout << "---------------------------------------" << endl;

    ifstream ifs(filename);
    string content((std::istreambuf_iterator<char>(ifs)),
                   (std::istreambuf_iterator<char>()));

    // send file to client in blocks
    send_blocks(client_socket, content, filesize, num_blocks);
    cout << "---------------------------------------" << endl;

    cout << "file send to client ..... " << endl;
    cout << "Closing connection ...... " << endl;
    cout << "---------------------------------------" << endl;
}
/*
    @name   --> get_file_ts
    @params --> char *filename
    @return --> last update timestamp of filename
    @desc   --> looks up for the last update timestamp of the filename and returns it

*/
time_t get_file_ts(const char *filename)
{
    string filename_str = string(filename);

    // open directory.txt
    ifstream fin_dir("./directory.txt");

    time_t final_update_ts;
    while (!fin_dir.eof())
    {

        string cur_filename;
        time_t cur_ts;

        fin_dir >> cur_filename >> cur_ts;

        cout << "---------------------------------------" << endl;

        cout << "getting the last update timestamp of the file " << endl;
        cout << cur_ts << endl;

        cout << "---------------------------------------" << endl;

        if (cur_filename == filename_str)
        {
            final_update_ts = cur_ts;
            break;
        }
    }

    cout << "---------------------------------------" << endl;

    cout << "Printing the final ts of the filename" << endl;
    cout << final_update_ts << endl;

    cout << "---------------------------------------" << endl;
    return final_update_ts;
}

/*
    @name   --> serve_client
    @params --> int client_socket
    @return --> void
    @desc   --> recieves the client socket and serves the requests of the client
*/
void serve_client(int client_socket)
{

    char datagram[1024];

    recv(client_socket, &datagram, sizeof(datagram), 0);

    // extract filename and header from datagram
    char header = datagram[0];

    string payload = string(datagram).substr(1, string(datagram).size());

    const char *filename = payload.c_str();

    // if operation is 0, then send file
    if (header == '0')
    {
        int file_length = get_length(filename);

        cout << "---------------------------------------" << endl;

        cout << "GOT FILE LENGTH = " << file_length << endl;
        cout << "---------------------------------------" << endl;

        send(client_socket, &file_length, sizeof(file_length), 0);

        cout << file_length << endl;

        send_file(client_socket, filename, file_length);
    }
    // else send the timestamp details of the file
    else if (header == '1')
    {
        cout << "---------------------------------------" << endl;

        cout << "Recieved request for timestamp" << endl;
        // get timestamp of filename
        time_t file_write_ts = get_file_ts(filename);

        // send timestamp of filename
        send(client_socket, &file_write_ts, sizeof(file_write_ts), 0);
    }

    // // close connection
    // close(client_socket);
}

/*
    Driver program
*/
int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (sockaddr *)&server_address, sizeof(server_address));

    // listern
    listen(server_socket, 5);

    // recieve connection
    while (true)
    {
        int client_socket = accept(server_socket, NULL, NULL);

        while (true)
        {
            serve_client(client_socket);

            fflush(stdout);
        }

        close(client_socket);
    }

    // socket struct
}