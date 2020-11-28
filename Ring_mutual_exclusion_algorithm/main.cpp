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
class Process
{
public:
    int id;
    bool is_active, needs_token;
    Process *next;

    Process(int id, bool needs_token)
    {
        this->id = id;
        this->is_active = true;
        this->next = nullptr;
        this->needs_token = needs_token;
    }

    void enter()
    {
        cout << "Process P" << this->id << " is entering in critical section" << endl
             << endl;
    }
};

int main()
{
    Process *p1 = new Process(1, true);
    Process *p2 = new Process(2, false);
    Process *p3 = new Process(3, true);
    Process *p4 = new Process(4, true);
    Process *p5 = new Process(5, false);
    Process *p6 = new Process(6, true);

    // make a ring
    p1->next = p6;
    p2->next = p1;
    p3->next = p2;
    p4->next = p3;
    p5->next = p4;
    p6->next = p5;
    p1->next = p6;

    Process *cur_process = p1;
    do
    {
        cout << "Process P" << cur_process->id << " has the token " << endl;
        if (cur_process->needs_token)
        {
            cur_process->enter();
        }
        else
        {
            cout << "Process P" << cur_process->id << " doesn't need the token" << endl
                 << endl;
        }
        cur_process = cur_process->next;
    } while (cur_process != p1);
}