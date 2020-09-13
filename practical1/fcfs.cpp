#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// declare all the jobs currently present in our system
vector<string> nameOfJobs = {"P1", "P2", "P3", "P4", "p5"};
vector<int> arrivalTimes = {0, 1, 2, 3, 4};
vector<int> burstTimes = {2, 6, 4, 9, 12};


// make a job class
class Job {
    public:
        string name;
        int arrivalTime, burstTime, waitingTime, completionTime;

        // constructor
        Job(string name, int arrivalTime, int burstTime){
            this->name = name;
            this->arrivalTime = arrivalTime;
            this->burstTime = burstTime;
        }

        // print the value of various properties of our job object
        void printJobStatistics(){
            cout  
            << setw(20) << this->name 
            << setw(5) << this->arrivalTime 
            << setw(5) << this->burstTime 
            << setw(5) << this->waitingTime 
            << setw(5) << this->completionTime 
            << endl;
        }
};

// provided the ready queue, this function will insert the jobs in the queue

void insertJobsInQueue(queue<Job> &readyQueue){

    // get the total number of jobs to insert
    int numberOfJobs = nameOfJobs.size();

    // insert jobs in the readyQueue
    for(int i=0; i<numberOfJobs; i++){

        // get its name, arrivalTime and BurstTime
        string nameOfCurJob = nameOfJobs[i];
        int arrivalTimeOfCurJob = arrivalTimes[i];
        int burstTimeOfCurJob = burstTimes[i];

        // create a new job using above attributes
        Job newJob(nameOfCurJob, arrivalTimeOfCurJob, burstTimeOfCurJob);

        // push the job to the readyQueue
        readyQueue.push(newJob);
    }

}

// function that will print important final information 
// the final table
// average waiting time of the system
// total processing time of the system
void printFinalInformation(vector<Job> finishedJobs, double averageWaitingTime, int totalProcessingTime){
    cout 
    << setw(20) << "Name" 
    << setw(5) << "A.T" 
    << setw(5) << "B.T" 
    << setw(5) << "W.T" 
    << setw(5) << "C.T" 
    << endl;
    for(auto job : finishedJobs){
        job.printJobStatistics();
    }

    // print the average waiting time of the system, total time to process the jobs
    cout << "Total time took to complete all the jobs in the ready queue = " << totalProcessingTime << " seconds" << endl;
    printf("Average waiting time of the system = %.2lf\n", averageWaitingTime);
}

int main()
{
    
    // create a ready queue
    queue<Job> readyQueue;

    // insert jobs in ready queue
    insertJobsInQueue(readyQueue);
    // jobQueue

    // counter keeps track of the booked time slot
    int counter = 0;

    // stores the finished jobs
    vector<Job> finishedJobs;

    // calculate the total waiting time of the processes
    int totalWaitingTime = 0;

    // process jobs while there are jobs available in ready queue
    // delay of counter time 
    while(!readyQueue.empty()){
        // get the first job from the readyQueue
        Job curJob = readyQueue.front(); readyQueue.pop();

        // calculate its waiting time
        curJob.waitingTime = (curJob.arrivalTime > counter) ? 0 : (counter - curJob.arrivalTime);

        // add Value of total waiting time
        totalWaitingTime += curJob.waitingTime;

        // get the time at which job will leave the system
        curJob.completionTime = curJob.arrivalTime + curJob.waitingTime + curJob.burstTime;

        // book a new time slot for the new job
        counter = curJob.completionTime;

        // since the job is completed, hence we can add the job in the finishedJobs array
        finishedJobs.push_back(curJob);
    }

    // print the job table 

    // calculate the average waiting time
    double averageWaitingTime = (1.0 * totalWaitingTime) / (nameOfJobs.size()); 

    printFinalInformation(finishedJobs, averageWaitingTime, counter);

    

}


