#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <climits>
#include <iomanip>

using namespace std;

class Process
{
public:
    int id, priority;
    bool is_active;
    Process *next;

    Process(int id, int priority)
    {
        this->id = id;
        this->priority = priority;
        this->is_active = true;
        this->next = nullptr;
    }
};

int main()
{
    // Create 6 processes
    Process *p1 = new Process(1, 1);
    Process *p2 = new Process(2, 5);
    Process *p3 = new Process(3, 6);
    Process *p4 = new Process(4, 7);
    Process *p5 = new Process(5, 4);
    Process *p6 = new Process(6, 17);

    // make a ring of these processes
    p1->next = p6;
    p2->next = p1;
    p3->next = p2;
    p4->next = p3;
    p5->next = p4;
    p6->next = p5;
    p1->next = p6;

    // p6 failed
    p6->is_active = false;

    // start election
    int token = INT_MIN;
    Process *cur_process = p6->next;

    cout << "Process Name\tPriority\tToken Value" << endl;
    while (token != cur_process->priority)
    {
        cout << "P" << cur_process->id << "\t\t" << cur_process->priority << "\t\t" << token << endl;
        if (cur_process->is_active)
            token = max(token, cur_process->priority);

        cur_process = cur_process->next;
    }

    // we have found our new coordinator
    cout << "Process P" << cur_process->id << " is the new coordinator having priority : " << cur_process->priority << endl;
}