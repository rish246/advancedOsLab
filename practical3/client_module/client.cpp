// #include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int PORT = 5400;

/*
    @name   --> print_str
    @params --> string str
    @return --> void
    @desc   --> prints the string str on the console

*/
void print_str(string str)
{
    for (char ch : str)
        printf("%c", ch);
    printf("\n");
}

/*
    @name   --> is_in_cache
    @params --> string file_name
    @return --> true/false
    @desc   --> looks in the cache_record.txt file to check whether the file is in the cache  

*/
bool is_in_cache(char *file_name)
{
    // ifstream fin("./cache/cache_record.txt", "r");
    ifstream fin("./cache/cache_record.txt");

    string filename_str(file_name);

    while (!fin.eof())
    {
        string cur_filename;
        fin >> cur_filename;
        if (cur_filename == file_name)
            return true;
    }

    return false;

    // return false;
}

/*
    @name   --> recieve_file
    @params --> int client_socket, int file_size, int num_blocks
    @return --> string file_content
    @desc   --> recieves num_blocks from server and store the blocks in a string... 


*/
string recieve_file(int client_socket, int file_size, int num_blocks)
{
    string file_content;
    int max_block_size = 1024;
    for (int i = 0; i < num_blocks; i++)
    {
        // fix bug in this code

        int cur_block_size = min(max_block_size, (file_size - (i * max_block_size)));

        char buffer[cur_block_size];

        recv(client_socket, &buffer, sizeof(buffer), 0);

        // printf("%s\n", buffer);

        string buffer_str(buffer);
        file_content += buffer_str;

        // printf("Block number %d recieved by the client\n", i);

        // printf("%s\n", buffer);
    }

    return file_content;
}

/*
    @name   --> extract_dirname
    @params --> string path
    @return --> string dirname
    @desc   --> extracts the name of the dir from the given path

*/
string extract_dirname(string path)
{
    string dirname = path;

    int loc_last_slash = 0;
    for (int i = 0; i < dirname.size(); i++)
    {
        if (dirname[i] == '/')
            loc_last_slash = i;
    }
    dirname = dirname.substr(0, loc_last_slash);

    return dirname;
}

/*
    @name   --> is_path_exits
    @params --> string dirname
    @return --> true/false
    @desc   --> Checks whether the path exists.. if it does, return true else false  

*/
bool is_path_exits(string path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

/*

    @name   -> cache file
    @params -> string filename, string filecontent
    @return -> void
    @desc   -> create a new file and writes the filecontent in the file in cache folder 


*/
void cache_file(string filename, string filecontent)
{
    // extract dir name
    string dirname = extract_dirname(filename);
    print_str(dirname);

    // check if the folder exists
    // update dirname so that the file is stored in the cache
    string dirname_cache = "./cache" + dirname.substr(1, dirname.size());

    print_str(dirname_cache);
    if (!is_path_exits(dirname_cache))
    {
        // if not then create the folder
        int status = mkdir(dirname_cache.c_str(), 0777);
        if (status != -1)
        {
            printf("Successfully created the directory\n");
        }

        else
        {
            printf("Couldn't create a new directory.. please try again\n");
        }
    }

    // create a file in the folder
    if (is_path_exits(dirname_cache))
    {
        string filename_cache = "./cache" + filename.substr(1, filename.size());
        // create a new file
        ofstream main_file(filename_cache, ios::out);

        // open the filename in the cache
        if (!main_file.is_open())
        {
            printf("Couldn't open the file... Error occured");
            return;
        }

        // start writing filecontent to file
        main_file << filecontent << endl;

        // create an entry in cache
        ofstream cache_record_file("./cache/cache_record.txt", ios::app);

        if (!cache_record_file.is_open())
        {
            printf("Couldn't open cache record file\n");
            return;
        }

        cache_record_file << filename << endl;

        main_file.close();

        cache_record_file.close();

        return;
    }

    printf("Couldn't find the folder to store the file\n");
}

// function print_str

// this is why we need cache coherence

// we changed file on the client

// didn't get latest data

int main()
{
    // ifstream fin("./cache/cache_record.txt", "r");
    char filename[] = "./folder2/file2.txt";

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // server object
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (!is_in_cache(filename))
    {
        // connect to the client
        connect(client_socket, (sockaddr *)&server_address, sizeof(server_address));

        // send name of file to server
        send(client_socket, filename, sizeof(filename), 0);

        // recieve length of file
        int file_size;
        recv(client_socket, &file_size, sizeof(int), 0);

        // revieve file from server
        // cout << file_size << endl;
        printf("%d\n", file_size);

        // create a buffer to store the file

        // recieve file
        int num_blocks;

        recv(client_socket, &num_blocks, sizeof(num_blocks), 0);
        printf("Num blocks to recieve = %d\n", num_blocks);

        // send acknowledgement
        bool is_reached = true;
        send(client_socket, &is_reached, sizeof(is_reached), 0);

        // recieve blocks
        string file_content = recieve_file(client_socket, file_size, num_blocks);

        cache_file(string(filename), file_content);

        printf("Finished recieving file from the server\n");

        close(client_socket);
    }
    else
    {
        printf("File already in cache\n");
    }

    return 0;
}