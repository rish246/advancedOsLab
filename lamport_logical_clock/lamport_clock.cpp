#include <iostream>
#include <vector>

using namespace std;

class Process
{
    void print_timer()
    {
        cout << "\tTimer of process p" << this->priority << " = " << this->timer << endl
             << endl;
    }

public:
    int timer = 0, priority, recieved_timer = 0;

    Process(int priority)
    {
        this->priority = priority;
    }

    void new_event()
    {
        this->timer++;
        this->print_timer();
    }

    // send a message to another process
    void send_message(Process &p)
    {
        cout << "\tMESSAGE SENT FROM PROCESS : P" << this->priority << " --> P" << p.priority << endl;

        cout << "\tORIGINAL TIMER OF PROCESS P" << p.priority << " : " << p.timer << endl;

        p.recieved_timer = this->timer;

        cout << "\tVALUE OF TIMER RECIEVED FROM PROCESS P" << this->priority << " : " << p.recieved_timer << endl;

        p.timer = max(p.recieved_timer, p.timer) + 1;
        cout << "\tFINAL VALUE OF TIMER : " << p.timer << endl;
    }
};

int main()
{
    Process p1(1), p2(2), p3(3);
    /* Event happening in P1 */
    p1.new_event();
    p1.new_event();

    /* Event happening in P2 */
    p2.new_event();
    p2.new_event();
    p2.new_event();

    /* Message sent by process p2 to process p1 */
    p2.send_message(p1);

    cout << "TIMER OF PROCESS P1 = " << p1.timer << endl;

    p3.new_event();
    p2.send_message(p3);

    cout << "TIMER OF PROCESS P3 = " << p3.timer << endl;
}