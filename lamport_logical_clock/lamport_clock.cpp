#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class event
{
public:
    int process_id, sequence_number, timer;

    event(int p_id, int s_no, int t) : process_id(p_id), sequence_number(s_no), timer(t) {}

    void print_stats()
    {
        cout << "e" << process_id << sequence_number << "(" << timer << ") --> ";
    }
};

class Process
{
public:
    int timer = 1, id, seq_number = 1;

    /* Each process maintains its local history */
    vector<event> local_history;

    Process(int _id) : id(_id) {}

    void new_event()
    {
        event e(this->id, this->seq_number, this->timer);

        local_history.push_back(e);

        this->timer++;

        this->seq_number++;
    }

    // send a message to another process
    void send_message(Process &p)
    {
        cout << "Message sent: P" << this->id << "(" << this->timer << ")"
             << " --> P" << p.id << "(" << p.timer << ")" << endl;

        this->new_event();

        p.timer = max(this->timer, p.timer);

        p.new_event();
    }
};

bool on_timer_or_id(const event &e1, const event &e2)
{
    if (e1.timer < e2.timer)
        return true;

    if (e1.timer == e2.timer)
        return (e1.process_id < e2.process_id);

    return false;
}

void print_vector(const vector<event> &v)
{
    for (event e : v)
    {
        e.print_stats();
    }
    cout << endl
         << endl;
}
int main()
{

    Process p1(1), p2(2);
    /* Event happening in P1 */
    p2.new_event();
    p1.new_event();
    p1.new_event();

    /* Event happening in P2 */
    p2.new_event();
    p2.new_event();

    // /* Message sent by process p2 to process p1 */
    p2.send_message(p1);

    p1.send_message(p2);

    /* construct the global history from the local histories */
    vector<event> global_history;

    /* global history = p1's local history U p2's local history */
    global_history.insert(global_history.end(), p1.local_history.begin(), p1.local_history.end());
    global_history.insert(global_history.end(), p2.local_history.begin(), p2.local_history.end());

    /* Sort all events on the basis of timer */
    cout << "Order of events in process P1" << endl;
    print_vector(p1.local_history);

    cout << "Order of events in process P2" << endl;
    print_vector(p2.local_history);

    cout << "Order of events in the whole system : " << endl;
    sort(global_history.begin(), global_history.end(), on_timer_or_id);
    print_vector(global_history);
}