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
        cout << "e" << process_id << sequence_number << "(timer = " << timer << ")"
             << " --> ";
    }
};

class Process
{
public:
    int timer = 1, priority, seq_number = 1;
    vector<event> process_events;

    Process(int priority)
    {
        this->priority = priority;
    }

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
        this->new_event();

        p.timer = max(this->timer, p.timer);

        p.new_event();
    }
};

bool on_timer_or_priority(event &e1, event &e2)
{
    if (e1.timer < e2.timer)
        return true;

    if (e1.timer == e2.timer)
        return (e1.process_id < e2.process_id);

    return false;
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
    sort(all_events.begin(), all_events.end(), on_timer_or_priority);

    for (event e : all_events)
    {
        e.print_stats();
    }
}