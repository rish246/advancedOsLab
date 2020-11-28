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

#define RELEASED 0
#define WANTED 1
#define HELD 2

bool is_worthy(int p1_id, int p2_id, int p1_Ts, int p2_Ts)
{
    if (p2_Ts < p1_Ts)
        return true;

    if (p2_Ts == p2_Ts)
        return p2_id <= p1_id;

    return false;
}

class Process
{
public:
    int id, Ts, state;

    queue<int> request_queue;

    int n_replies = 0;

    Process(int id, int Ts)
    {
        this->id = id;
        this->Ts = Ts;
        this->state = RELEASED;
    }

    void multicase_message(const vector<Process> &group)
    {
        cout << "---------------------------------------------------------------------" << endl;
        cout << "----------------------- MULTICASTING MESSAGE -------------------" << endl;

        cout << "Process P" << this->id << " is multicasting message to the entire group" << endl;
        for (Process p : group)
        {
            cout << "Message sent to process : P" << p.id << endl;
            n_replies += p.recv_message(this->id, this->Ts);
        }
        cout << "----------------------------------------------------------------------" << endl;
    }

    int recv_message(int id, int Ts)
    {

        if ((this->state == HELD) || ((this->state == WANTED) && is_worthy(id, this->id, Ts, this->Ts)))
        {
            request_queue.push(this->id);
            return 0;
        }

        return 1;
    }

    void enter()
    {
        cout << "---------------------------------------------------------------------" << endl;
        cout << "----------------------- ENTERING CRITICAL SECTION -------------------" << endl;
        this->state = HELD;
        cout << "Process P" << this->id << " is entering the critical section" << endl;
        cout << "Process P" << this->id << " is performing operations" << endl
             << endl;
    }

    void release()
    {
        this->state = RELEASED;
        cout << "----------------------- RELEASING CRITICAL SECTION -------------------" << endl;

        cout << "Process P" << this->id << " is releasing the critical section" << endl;

        // reply to all the pending requests
        cout << "Process P" << this->id << " is replying to all the pending requests " << endl;

        while (!this->request_queue.empty())
        {
            cout << "\t=> Reply sent to the process P" << this->request_queue.front() << endl;
            this->request_queue.pop();
        }

        cout << "---------------------------------------------------------------------" << endl;
    }
};

int main()
{
    cout << endl
         << "C++ IMPLEMENTATION OF RICART-AGRAWAL ALGORITHM" << endl;
    // create 3 processes and group of processes
    Process p1(1, 34), p2(2, 41), p3(3, 24);

    // processes p1 and p2 wants to enter the critical section
    p1.state = WANTED;
    p2.state = WANTED;

    // p1 multicast the message in the group
    vector<Process> group = {p1, p2, p3};

    // p1 multicasts a message to the group
    // the message consists of p1's id and its Ts
    p1.multicase_message(group);
    // lets say .. there were some pending requests in p1 earlier
    p1.request_queue.push(2);

    // if number of replies == (N - 1)
    // then p1 will enter the critical section and perform the operation
    // upon release, it will reply to all the pending requests
    // then, it will change the state to RELEASED
    if (p1.n_replies == (int)group.size() - 1)
    {
        p1.enter();
        p1.release();
    }

    return 0;
}
