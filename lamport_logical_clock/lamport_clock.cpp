#include <iostream>
#include <vector>

using namespace std;

class event
{
public:
    int process_id, sequence_number, timer;

    event(int p_id, int s_no, int t) : process_id(p_id), sequence_number(s_no), timer(t) {}

    void print_stats()
    {
        cout << "e" << process_id << sequence_number << "( timer = " << timer << ")"
             << " --> ";
    }
};

vector<event> all_events;

class Process
{

public:
    int timer = 1, priority, seq_number = 1;

    Process(int priority)
    {
        this->priority = priority;
    }

    void new_event()
    {
        event e(this->priority, this->seq_number, this->timer);

        all_events.push_back(e);

        this->timer++;

        this->seq_number++;
    }

    // send a message to another process
    void send_message(Process &p)
    {
        this->new_event();

        p.timer = max(this->timer, p.timer);

        p.new_event();
    }
};

int main()
{

    Process p1(1), p2(2);
    /* Event happening in P1 */
    p1.new_event();
    p2.new_event();
    p1.new_event();

    /* Event happening in P2 */
    p2.new_event();
    p2.new_event();

    // /* Message sent by process p2 to process p1 */
    p2.send_message(p1);

    p1.send_message(p2);

    for (event e : all_events)
    {
        e.print_stats();
    }
    cout << " END" << endl;
    // cout << "TIMER OF PROCESS P1 = " << p1.timer << endl;

    // p3.new_event();
    // p2.send_message(p3);

    // cout << "TIMER OF PROCESS P3 = " << p3.timer << endl;
}