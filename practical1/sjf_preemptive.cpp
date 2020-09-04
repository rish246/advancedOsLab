#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


vector<string> nameOfJobs = {"P1", "P2", "P3", "P4", "P5"};
vector<int> arrivalTimes = {2, 5, 1, 0, 4};
vector<int> burstTimes = {6, 2, 8, 3, 4};

// max time for which our cpu will process the jobs
const int inf = 100;


class Job {
public:
    string name;
    bool served = false;
    int arrivalTime, burstTime, waitingTime, responseTime, completionTime;

    Job(string name, int arrivalTime, int burstTime){
        this->name = name;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->waitingTime  = 0;
        this->completionTime = 0;
        this->responseTime = 0;
    }

    void printStats(){
        cout  << setw(20) << this->name << setw(5) << this->arrivalTime << setw(5) << this->burstTime << setw(5) << this->waitingTime << setw(5) << this->completionTime << setw(5) << this->responseTime << endl;
    }
};

// helper used to insert jobs in the job queue
void insertJobsInJobQueue(vector<Job> &jobQueue)
{
    int numberOfJobs = nameOfJobs.size();

    for(int i=0; i<numberOfJobs; i++){
        string nameOfCurJob = nameOfJobs[i];
        int arrivalTimeOfCurJob = arrivalTimes[i];
        int burstTimeOfCurJob = burstTimes[i];

        Job newJob(nameOfCurJob, arrivalTimeOfCurJob, burstTimeOfCurJob);
        jobQueue.push_back(newJob);
    }
    
    
}


// helper used to insert jobs in the ready queue
void insertJobsInReadyQueue(vector<Job*> &readyQueue, vector<Job> &jobQueue, int counter){

    // check for all the jobs which are arrived before counter
    // passed in original jobQueue
    for(auto &job : jobQueue){
        if(job.arrivalTime <= counter && job.served == false){
            Job* jobptr = &job;
            readyQueue.push_back(jobptr);
        }
    }

}

// comparator used for sorting the jobs in ready queue
bool onBurstTime(Job* j1, Job* j2){
    return j1->burstTime <= j2->burstTime;
}

// function that will serve the job
// calculate the average waiting time
// completion time
// response time for that job
void serveJob(Job* jobToServe, int counter){
    jobToServe->served = true;


        // counter += jobToServe->burstTime;
    jobToServe->waitingTime = counter - jobToServe->arrivalTime;


    jobToServe->completionTime = counter + jobToServe->burstTime;

    


    jobToServe->responseTime = jobToServe->waitingTime;
}

// function that will print the final information
void printFinalInformation(vector<Job> &finishedJobs, int totalWaitingTime){
    double averageWaitingTime = (1.0 * totalWaitingTime) / (nameOfJobs.size());

    cout << setw(20) << "Name" << setw(5) << "A.T" << setw(5) << "B.T" << setw(5) << "W.T" << setw(5) << "C.T" << setw(5) << "R.T" << endl;

    for(auto job : finishedJobs) job.printStats();
    printf("Average waiting time in the system = %.2lf\n", averageWaitingTime);
}


int main()
{
    vector<Job> jobQueue = {}, finishedJobs = {};
    insertJobsInJobQueue(jobQueue);

    // sort appropriately 
    // sort(jobQueue.begin(), jobQueue.end(), onPriority);

    
    int counter = 0;
    int totalWaitingTime = 0;

    while(counter < inf){
        // address of all job objects
        vector<Job*> readyQueue;

        // this code is wrong
        insertJobsInReadyQueue(readyQueue, jobQueue, counter);

        // if no job has arrived, then don't do anything
        if(readyQueue.size() == 0){
            counter++;
            continue;
        } 

        // sort based on birst time
        sort(readyQueue.begin(), readyQueue.end(), onBurstTime);

        // // get the first job
        // the first job is the job with min burst time
        // select the job for processing
        Job* jobToServe = readyQueue[0]; // address of the job to serve

        // provide service to the selected job
        serveJob(jobToServe, counter);

        // increase the counter to the completion time value
        // cpu will be busy till the current job is served
        // so till then, add the arriving jobs in readyQueue
        counter = jobToServe->completionTime;


        totalWaitingTime += jobToServe->waitingTime;


        // once processed, add the job to finished jobs array
        finishedJobs.push_back(*jobToServe);

        




    }
    


    // print all the necessary information like:
    //      1. Table of information
    //      2. Average waiting time
    printFinalInformation(finishedJobs, totalWaitingTime);
    

}