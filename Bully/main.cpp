#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <climits>
#include <iomanip>
#include <queue>

using namespace std;
#define MESSAGE_ELECTION 0
#define MESSAGE_COORDINATOR 1

int num_processes = 4;

class Process
{
public:
    int id, n_replies, is_active;
    // constructor
    Process(int id)
    {
        this->id = id;
        this->n_replies = 0;
        this->is_active = 1;
    }

    // process starting election
    void start_election(vector<Process> &group)
    {
        cout << "\t\t Process P" << this->id << " is starting the election" << endl;

        for (int i = this->id + 1; i < group.size(); i++)
        {
            cout << "P" << this->id << " --> Election message sent to process P" << i << endl;
            // send an election message to processes to the right of current process
            // count the number of replies recieved from them
            // each process to the right will receive the message from current process
            int process_response = group[i].recieve_message(MESSAGE_ELECTION, this->id);
            if (process_response == 1)
            {
                this->n_replies++;
                cout << "P" << this->id << " --> Accept message recieved from process P" << group[i].id << endl;
            }
        }
    }

    void send_coordinator_message(vector<Process> &group)
    {
        cout << "-------------------------------------------------------------------------------------" << endl;
        cout << "\t\tProcess P" << this->id << " is the new coordinator" << endl;
        cout << "-------------------------------------------------------------------------------------" << endl;

        cout << "P" << this->id << " --> Sending coordinator message to the group" << endl;
        for (int i = this->id - 1; i >= 0; i--)
        {
            group[i].recieve_message(MESSAGE_COORDINATOR, this->id);
        }
    }

    int recieve_message(int type, int id)
    {
        if (type == MESSAGE_ELECTION)
        {
            return (this->is_active);
        }
        // type == message coordinator
        else if (type == MESSAGE_COORDINATOR)
        {
            cout << "P" << this->id << " --> Coordinator message recieved from Process : P" << id << endl;
        }
        return 0;
    }
};

int main()
{
    // create 4 processes
    vector<Process> group;

    // create process
    for (int i = 0; i < num_processes; i++)
    {
        group.push_back(Process(i));
    }

    // process P4 was the coordinator, but it failed
    // chose the new coordinator
    group[group.size() - 1].is_active = 0;

    for (int i = 0; i < group.size(); i++)
    {
        cout << "-------------------------------------------------------------------------------------" << endl;
        Process &cur_process = group[i];

        // current process will start the election
        cur_process.start_election(group);

        // if it recieved any replies in the time bound, it is not the coordinator and process continues
        if (cur_process.n_replies > 0)
        {
            cout << "Process P" << cur_process.id << " recieved " << cur_process.n_replies << " replies. Hence it cannot be the coordinator" << endl;
            cout << "-------------------------------------------------------------------------------------" << endl;

            continue;
        }

        // send coordinator message to the group
        cur_process.send_coordinator_message(group);
        cout << "-------------------------------------------------------------------------------------" << endl;

        // end the election
        break;
    }

    return 0;
}