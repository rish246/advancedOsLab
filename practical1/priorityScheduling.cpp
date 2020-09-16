#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<string> namesOfProcesses = {"P1", "P2", "P3", "P4", "p5"};
vector<int> arrivalTimes = {0, 1, 2, 3, 4};
vector<int> burstTimes = {2, 6, 4, 9, 12};
vector<int> priorities = {2, 3, 4, 5, 5};

const int INFINITY = 100;

class Job {
public:
    string name;
    bool isServed = false, isPresentInReadyQueue = false;
    int arrivalTime, burstTime, waitingTime, completionTime, turnAroundTime, originalBurstTime, priority;

    Job(string name, int arrivalTime, int burstTime){
        this->name = name;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->waitingTime  = 0;
        this->completionTime = 0;
        this->turnAroundTime = 0;
        this->isServed = false;
        this->originalBurstTime = this->burstTime;
    }

    

    void printStats(){
        cout  
        << setw(20) << this->name 
        << setw(5) << this->arrivalTime
        << setw(5) << this->originalBurstTime
        << setw(5) << this->waitingTime 
        << setw(5) << this->completionTime 
        << setw(5) << this->turnAroundTime
        << endl;
    }
};

struct ComparyPriorities
{
    /* data */
    bool operator () (Job* j1, Job* j2){
        return j1->priority < j2->priority;
    }
};


void insertJobsInJobQueue(vector<Job> &jobQueue)
{
    int numberOfJobs = namesOfProcesses.size();

    for(int i=0; i<numberOfJobs; i++){
        string nameOfCurJob = namesOfProcesses[i];
        int arrivalTimeOfCurJob = arrivalTimes[i];
        int burstTimeOfCurJob = burstTimes[i];
        int priorityOfCurJob = priorities[i];

        Job newJob(nameOfCurJob, arrivalTimeOfCurJob, burstTimeOfCurJob);
        newJob.priority = priorityOfCurJob;
        jobQueue.push_back(newJob);
    }
    
    
}

void insertJobsInReadyQueue(priority_queue<Job*, vector<Job*>, ComparyPriorities> &readyQueue, vector<Job> &jobQueue, int timer){

    // check for all the jobs which are arrived before timer
    // passed in original jobQueue
    for(auto &job : jobQueue){
        // check if job has arrived and has not been isisServed yed
        if(job.arrivalTime <= timer && job.isServed == false && job.isPresentInReadyQueue == false){
            Job* jobptr = &job;
            // push address of job in the ready queue
            jobptr->isPresentInReadyQueue = true;
            readyQueue.push(jobptr);
        }
    }

}

void serveJob(Job* jobToServe, int timer){
    jobToServe->isServed = true;

    jobToServe->completionTime = timer;

    jobToServe->turnAroundTime = jobToServe->completionTime - jobToServe->arrivalTime;

    jobToServe->waitingTime = jobToServe->turnAroundTime - jobToServe->originalBurstTime;
}

void printFinalInformation(vector<Job> &finishedJobs, int totalWaitingTime, int totalTurnAroundTime){

    int totalNumberOfJobs = namesOfProcesses.size();
    double averageWaitingTime = (1.0 * totalWaitingTime) / (totalNumberOfJobs);
    double averageTurnAroundTime = (1.0 * totalTurnAroundTime) / (totalNumberOfJobs);
    cout 
    << setw(20) << "Name" 
    << setw(5) << "A.T" 
    << setw(5) << "B.T" 
    << setw(5) << "W.T" 
    << setw(5) << "C.T" 
    << setw(5) << "T.A.T" 
    << endl;

    for(auto job : finishedJobs) job.printStats();
    printf("Average turn around time in the system = %.2lf\n", averageTurnAroundTime);
    printf("Average waiting time in the system = %.2lf\n", averageWaitingTime);
}



int main()
{
    vector<Job> jobQueue = {}, finishedJobs = {};

    insertJobsInJobQueue(jobQueue);


    int timer = 0;
    priority_queue<Job*, vector<Job*>, ComparyPriorities> readyQueue; // when something is entered, it gets to its appr location

    int totalWaitingTime = 0, totalTurnAroundTime = 0;

    while(timer < INFINITY){


        // insert jobs in ready queue
        insertJobsInReadyQueue(readyQueue, jobQueue, timer);


        // get the best job
        Job* jobToServe = readyQueue.top(); 


        jobToServe->burstTime--;

        timer++;


        if(jobToServe->burstTime == 0){
            // done processing .... no need for more cpu allocation 
            // out of our readyQueue
            readyQueue.pop();

            serveJob(jobToServe, timer);

            totalWaitingTime += jobToServe->waitingTime;

            totalTurnAroundTime += jobToServe->turnAroundTime;

            finishedJobs.push_back(*jobToServe);
        }

        
    }

    printFinalInformation(finishedJobs, totalWaitingTime, totalTurnAroundTime);


}


// why am i getting waiting time == (turnAroundTime)

// higher average waiting time in round_robin.cpp