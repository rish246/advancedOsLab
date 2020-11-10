#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class event
{
public:
    int process_id, sequence_number, timer;

    event(int p_id, int s_no, int t) : process_id(p_id), sequence_number(s_no), timer(t) {}

    void print_stats()
    {
        cout << "e" << process_id << sequence_number << "(" << timer << ")"
             << " --> ";
    }
};

class Process
{
public:
    int timer = 1, priority, seq_number = 1;

    /* List of events happening in this process */
    vector<event> process_events;

    Process(int _priority) : priority(_priority) {}

    void new_event()
    {
        event e(this->priority, this->seq_number, this->timer);

        process_events.push_back(e);

        this->timer++;

        this->seq_number++;
    }

    // send a message to another process
    void send_message(Process &p)
    {
        cout << "Message sent: P" << this->priority << "(" << this->timer << ")"
             << " --> P" << p.priority << "(" << p.timer << ")" << endl;

        this->new_event();

        p.timer = max(this->timer, p.timer);

        p.new_event();
    }
};

bool on_timer_or_priority(const event &e1, const event &e2)
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

    vector<event> all_events;

    all_events.insert(all_events.end(), p1.process_events.begin(), p1.process_events.end());
    all_events.insert(all_events.end(), p2.process_events.begin(), p2.process_events.end());

    /* Sort all events on the basis of timer */
    cout << "Order of events in process P1" << endl;
    print_vector(p1.process_events);

    cout << "Order of events in process P2" << endl;
    print_vector(p2.process_events);

    cout << "Order of events in the whole system : " << endl;
    sort(all_events.begin(), all_events.end(), on_timer_or_priority);
    print_vector(all_events);
}