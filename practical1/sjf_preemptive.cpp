#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


vector<string> nameOfJobs = {"P1", "P2", "P3", "P4"};
vector<int> arrivalTimes = {1, 2, 1, 4};
vector<int> burstTimes = {3, 4, 2, 4};

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

bool onBurstTime(Job* j1, Job* j2){
    return j1->burstTime <= j2->burstTime;
}

int main()
{
    vector<Job> jobQueue = {}, finishedJobs = {};
    insertJobsInJobQueue(jobQueue);

    // sort appropriately 
    // sort(jobQueue.begin(), jobQueue.end(), onPriority);

    
    int counter = 0;

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
        sort(readyQueue.begin(), readyQueue.end(), onBurstTime);

        // // get the first job
        // Job *jobToServe = &readyQueue[0];
        Job* jobToServe = readyQueue[0]; // address of the job to serve

        // cout << jobToServe->served << endl;

        // // cout << jobToServe.name << endl;
        jobToServe->served = true;


        // counter += jobToServe->burstTime;

        jobToServe->completionTime = counter + jobToServe->burstTime;

        counter = jobToServe->completionTime;
        
    
        jobToServe->waitingTime = counter - jobToServe->arrivalTime;

        jobToServe->responseTime = jobToServe->waitingTime;

        finishedJobs.push_back(*jobToServe);

        




    }
    
    for(auto job : finishedJobs) job.printStats();


}