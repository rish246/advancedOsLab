#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<string> namesOfProcesses = {"P1", "P2", "P3", "P4", "P5", "P6"};
vector<int> arrivalTimes = {5, 4, 3, 1, 2, 6};
vector<int> burstTimes = {5, 6, 7, 9, 2, 3};

const int INFINITY = 100;
const int TIME_INTERVAL = 3;

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

void serveJob(Job* jobToServe, int timer){
    jobToServe->isServed = true;
        // mark isisServed as true
    // these are calculated accurately
    jobToServe->completionTime = timer;
// calculate waiting time, execution time and tat
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
        if(jobToServe->burstTime != 0){
            prevJobProcessed = jobToServe;
            continue;
        }
            

            
        // burstTime == 0 
        serveJob(jobToServe, timer);

        // cout << jobToServe->name << ' ' << jobToServe->burstTime << endl;
        totalWaitingTime += jobToServe->waitingTime;

        totalTurnAroundTime += jobToServe->turnAroundTime;

        finishedJobs.push_back(*jobToServe);

    }

    printFinalInformation(finishedJobs, totalWaitingTime, totalTurnAroundTime);


}


// why am i getting waiting time == (turnAroundTime)