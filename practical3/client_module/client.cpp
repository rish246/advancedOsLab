#include <fstream>
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
        time_t timestamp;
        fin >> cur_filename >> timestamp;

        if (cur_filename == file_name)
        {

            cout << "---------------------------------------" << endl;

            cout << "file " << filename_str << " found in cache" << endl;
            cout << "---------------------------------------" << endl;

            return true;
        }
    }

    return false;

    // return false;
}

/*
    @name   --> extract_dirname
    @params --> string path
    @return --> string dirname
    @desc   --> extracts the name of the dir from the given path
*/
string extract_dirname(const string &path)
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
void cache_file(string filename, const string &filecontent)
{
    // extract dir name
    string dirname = extract_dirname(filename);

    // check if the folder exists
    // update dirname so that the file is stored in the cache
    string dirname_cache = "./cache" + dirname.substr(1, dirname.size());

    if (!is_path_exits(dirname_cache))
    {
        // if not then create the folder
        int status = mkdir(dirname_cache.c_str(), 0777);
        if (status != -1)
        {
            cout << "---------------------------------------" << endl;
            cout << "Successfully Created a directory" << endl;
            cout << "---------------------------------------" << endl;
        }

        else
        {
            cout << "---------------------------------------" << endl;

            printf("Couldn't create a new directory.. please try again\n");
            cout << "---------------------------------------" << endl;
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
            cout << "---------------------------------------" << endl;

            printf("Couldn't open the file... Error occured");

            cout << "---------------------------------------" << endl;

            return;
        }

        // start writing filecontent to file
        main_file << filecontent << endl;

        // create an entry in cache
        ofstream cache_record_file("./cache/cache_record.txt", ios::app);

        /*

        @TODO -> make changes in the construction of a cache record
        @Fields -> filename timestamp


        */
        time_t cache_timestamp = time(NULL);

        cache_record_file << filename << ' ' << cache_timestamp << endl;

        main_file.close();

        cache_record_file.close();

        return;
    }

    printf("Couldn't find the folder to store the file\n");
}

/*
    @name   --> recieve_timestamp_from_server
    @params --> int client_socket, string filename
    @return --> timestamp when the file was last updated on the server
    @desc   --> gets the last update time stamp of a file and returns it to the main program 
*/
time_t recieve_timestamp_from_server(int client_socket, const char *filename)
{
    // cout << "Func called" << endl;

    time_t last_update_timestamp = time(NULL);
    // header = '1'
    int filename_len = string(filename).size();

    int datagram_len = filename_len + 1;

    // construct header
    char header = '1';

    // datagram
    string datagram = header + string(filename);

    // final datagram
    const char *timestamp_datagram = datagram.c_str();

    printf("%s\n", timestamp_datagram);

    // send datagram
    if (send(client_socket, timestamp_datagram, datagram_len, 0) < 0)
    {
        cout << "---------------------------------------" << endl;

        cerr << "Couldn't send the timestamp datagram" << endl;

        cout << "---------------------------------------" << endl;

        return time(NULL);
    }

    cout << "sent timestamp_datagram" << endl;

    // recv ts
    recv(client_socket, &last_update_timestamp, sizeof(last_update_timestamp), 0);

    // recieve time_t

    return last_update_timestamp;
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
    @name   --> get_file_cache_ts
    @params --> string filename
    @return --> timestamp when the file was cached on our client module
    @desc   --> searches for filename in client record, notes the last record timestamp and returns it 
*/
time_t get_file_cache_ts(const char *filename)
{

    // look into cache_dir
    string filename_str(filename);

    ifstream fin_cache("./cache/cache_record.txt");

    time_t cache_file_ts;
    while (!fin_cache.eof())
    {

        string cur_filename;
        time_t cur_ts;

        fin_cache >> cur_filename >> cur_ts;

        if (cur_filename == filename_str)
        {
            cout << "---------------------------------------" << endl;

            cout << cur_filename << '\n'
                 << cur_ts << endl;

            cout << "---------------------------------------" << endl;

            return cur_ts;
        }
    }

    return cache_file_ts;

    // if filename == cur_filename

    // return ts
}

/*
    @name   --> update_cache
    @params --> string filename, int new_ts, int old_ts
    @return --> void
    @desc   --> updates the timestamp of the filename in the cache record (replaces old_ts with new_ts) 
*/

void update_cache(const char *filename, time_t new_ts, time_t old_ts)
{
    // udate the cache so its holds the latest ts of updation
    ifstream fin("./cache/cache_record.txt");

    string content = "";

    // read content of file in better way
    while (!fin.eof())
    {

        string fn, ts;

        fin >> fn >> ts;

        content += fn + ' ' + ts;
    }

    // find old
    string to_remove = string(filename) + ' ' + to_string(old_ts);

    cout << "............ Remove string ....................." << endl;
    cout << to_remove << endl;
    cout << "................................................." << endl;

    string to_insert = string(filename) + ' ' + to_string(new_ts);

    cout << "................ Insert string .................." << endl;
    cout << to_insert << endl;
    cout << "................................................." << endl;
    // remove string from content
    int pos = content.find(to_remove);

    if (pos != string::npos)
    {

        cout << "------ record found at string -----------" << endl;

        cout << pos << endl;

        cout << "-------------------------------------------" << endl;

        // to_remove ... not being removed
        content.erase(pos, to_remove.size());

        content += to_insert;
    }
    cout << "File content of cache record" << endl;
    cout << "..............................." << endl;
    cout << content << endl;
    cout << "..............................." << endl;

    fin.close();

    ofstream fout("./cache/cache_record.txt");

    fout << content << endl;

    fout.close();
}
/*
    @name   --> get_file_from_server
    @params --> int client_socket, string filename
    @return --> string filecontent
    @desc   --> sends the filename to the server, recieves the filecontent. Then it returns the filecontent 
*/
string get_file_from_server(int client_socket, char filename[])
{
    printf("Filename sending to the server = %s\n", filename);

    /////////////////////////////////////////////////////
    /*
    @TODO --> add a header to the filename 
            ==> header shows the type of request being sent to the server
    */
    int filename_len = string(filename).size();

    int datagram_len = filename_len + 1;
    char header = '0';
    // send the whole header

    // construct header
    string datagram = header + string(filename);

    const char *filename_datagram = datagram.c_str();

    printf("%s\n", filename_datagram);

    send(client_socket, filename_datagram, datagram_len, 0);

    cout << "-----------------------------------------" << endl;

    cout << "Filename datagram sent to the server" << string(filename_datagram) << endl;
    /////////////////////////////////////////////////
    cout << "---------------------------------------" << endl;

    // // recieve length of file
    int file_size;
    recv(client_socket, &file_size, sizeof(file_size), 0);

    // // // revieve file from server
    cout << file_size << endl;

    int num_blocks;

    recv(client_socket, &num_blocks, sizeof(num_blocks), 0);

    cout << "---------------------------------------" << endl;

    printf("Num blocks to recieve = %d\n", num_blocks);
    cout << "---------------------------------------" << endl;

    // // // send acknowledgement
    bool is_reached = true;
    send(client_socket, &is_reached, sizeof(is_reached), 0);

    // // recieve blocks
    string file_content = recieve_file(client_socket, file_size, num_blocks);
    // cache file after recieving

    return file_content;
}

/*
Driver Program
*/

int main()
{
    // ifstream fin("./cache/cache_record.txt", "r");
    char filename[] = "./folder1/file_to_recieve.txt";

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
        string file_content = get_file_from_server(client_socket, filename);

        cache_file(string(filename), file_content);

        fflush(stdout);

        close(client_socket);
    }
    else
    {
        connect(client_socket, (sockaddr *)&server_address, sizeof(server_address));

        cout << "Require timestamp" << endl;

        time_t last_update_ts = recieve_timestamp_from_server(client_socket, filename);

        time_t client_cache_ts = get_file_cache_ts(filename);

        if (difftime(last_update_ts, client_cache_ts) <= 0)
        {

            cout << "---------------------------------------" << endl;

            cout << "File up to date" << endl;

            cout << "---------------------------------------" << endl;
        }
        else
        {

            cout << "---------------------------------------" << endl;

            cout << "File updated at server " << endl;

            cout << "---------------------------------------" << endl;
            // close(client_socket);
            // make sure that file is fetched in this case
            connect(client_socket, (sockaddr *)&server_address, sizeof(server_address));

            string file_content = get_file_from_server(client_socket, filename);

            // this func not working
            // make sure the timestamps are rig
            update_cache(filename, last_update_ts, client_cache_ts);

            close(client_socket);
            // update_cache(filename, )
        }
    }
    return 0;
}