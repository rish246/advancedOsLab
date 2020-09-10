#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<string> namesOfProcesses = {"P1", "P2", "P3", "P4", "P5"};
vector<int> arrivalTimes = {0, 1, 2, 3, 4};
vector<int> burstTimes = {5, 3, 1, 2, 3};

const int INFINITY = 100;
const int TIME_INTERVAL = 2;

class Job {
public:
    string name;
    bool isServed = false, isAlreadyInReadyQueue = false;
    int arrivalTime, burstTime, waitingTime, completionTime, turnAroundTime, originalBurstTime;

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

void insertJobsInJobQueue(vector<Job> &jobQueue)
{
    int numberOfJobs = namesOfProcesses.size();

    for(int i=0; i<numberOfJobs; i++){
        string nameOfCurJob = namesOfProcesses[i];
        int arrivalTimeOfCurJob = arrivalTimes[i];
        int burstTimeOfCurJob = burstTimes[i];

        Job newJob(nameOfCurJob, arrivalTimeOfCurJob, burstTimeOfCurJob);
        jobQueue.push_back(newJob);
    }
    
    
}

// void insertJobsInReadyQueue(queue<Job*> &readyQueue, vector<Job> jobQueue, int timer)
// {
//     // search in jobQueue, check for arrived processes
//     for(auto &job : jobQueue){
//         if(job.arrivalTime <= timer && job.isisisServed == false){
//             Job* jobptr = &job;
//             readyQueue.push(jobptr);
//         }
//     }

//     // insert in readyQueue
// }

void insertJobsInReadyQueue(queue<Job*> &readyQueue, vector<Job> &jobQueue, int timer){

    // check for all the jobs which are arrived before timer
    // passed in original jobQueue
    for(auto &job : jobQueue){
        // check if job has arrived and has not been isisServed yed
        if(job.arrivalTime <= timer && job.isServed == false && job.isAlreadyInReadyQueue == false){
            Job* jobptr = &job;
            // push address of job in the ready queue
            jobptr->isAlreadyInReadyQueue = true;
            readyQueue.push(jobptr);
        }
    }

}

bool onArrivalTime(Job j1, Job j2){
    return j1.arrivalTime < j2.arrivalTime;
}

int main()
{
    vector<Job> jobQueue = {}, finishedJobs = {};

    insertJobsInJobQueue(jobQueue);

    // start the processing
    sort(jobQueue.begin(), jobQueue.end(), onArrivalTime);

    int timer = 0;

    // stores the addresses of the jobs

    queue<Job*> readyQueue;

    // store the job processes in previous step
    Job* prevJobProcessed = NULL;


    int totalWaitingTime = 0;
    int totalTurnAroundTime = 0;

    while(timer < INFINITY){
        
        insertJobsInReadyQueue(readyQueue, jobQueue, timer);

        // insert the prevJobProcesses
        if(prevJobProcessed != NULL) readyQueue.push(prevJobProcessed);

        prevJobProcessed = NULL;

        if(readyQueue.empty()){
            timer += 1;
            continue;
        }

        /// if some jobs have arrived

        // take first job of the queue
        Job* jobToServe = readyQueue.front(); readyQueue.pop();
        // process the job for interval
        
        int processingInterval = min(TIME_INTERVAL, jobToServe->burstTime);

        jobToServe->burstTime -= processingInterval;

        timer += processingInterval;

        // check if job has done its processing

        // job will not be pushed in ready queue again
        if(jobToServe->burstTime == 0){
            // if yes, push it into finished jobs
            jobToServe->isServed = true;
        // mark isisServed as true
            // these are calculated accurately
            jobToServe->completionTime = timer;
        // calculate waiting time, execution time and tat
            jobToServe->turnAroundTime = jobToServe->completionTime - jobToServe->arrivalTime;

            jobToServe->waitingTime = jobToServe->turnAroundTime - jobToServe->originalBurstTime;

            // cout << jobToServe->name << ' ' << jobToServe->burstTime << endl;
            totalWaitingTime += jobToServe->waitingTime;

            totalTurnAroundTime += jobToServe->turnAroundTime;

            finishedJobs.push_back(*jobToServe);


        }
        
        else {
            // we still have some processing to do
            // store it in a global variable
            prevJobProcessed = jobToServe;

            // access the variable in next iteration of the loop
        }
        
        // we have to append the process to the end of the readyQueue

    }

    double averageWaitingTime = (1.0 * totalWaitingTime) / (namesOfProcesses.size());
    double averageTurnAroundTime = (1.0 * totalTurnAroundTime) / (namesOfProcesses.size());

    cout << setprecision(5) << averageTurnAroundTime << endl;
    cout << setprecision(5) << averageWaitingTime << endl;
    for(auto job : finishedJobs) job.printStats();


}


// why am i getting waiting time == (turnAroundTime)