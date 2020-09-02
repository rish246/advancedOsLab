#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// declare all the jobs currently present in our system
vector<string> nameOfJobs = {"MsWord", "NotePad", "Vscode"};
vector<int> arrivalTimes = {0, 0, 0};
vector<int> burstTimes = {10, 5, 8};


// make a job class
class Job {
    public:
        string name;
        int arrivalTime, burstTime, waitingTime, completionTime;

        Job(string name, int arrivalTime, int burstTime){
            this->name = name;
            this->arrivalTime = arrivalTime;
            this->burstTime = burstTime;
        }

        void printJobStatistics(){
            cout  << setw(20) << this->name << setw(5) << this->arrivalTime << setw(5) << this->burstTime << setw(5) << this->waitingTime << setw(5) << this->completionTime << endl;
        }
};

// provided the ready queue, this function will insert the jobs in the queue

void insertJobsInQueue(queue<Job> &readyQueue){

    int numberOfJobs = nameOfJobs.size();

    for(int i=0; i<numberOfJobs; i++){
        string nameOfCurJob = nameOfJobs[i];
        int arrivalTimeOfCurJob = arrivalTimes[i];
        int burstTimeOfCurJob = burstTimes[i];

        // create a new job using above attributes
        Job newJob(nameOfCurJob, arrivalTimeOfCurJob, burstTimeOfCurJob);
        readyQueue.push(newJob);
    }

}

int main()
{
    
    // insert jobs in ready queue
    queue<Job> readyQueue;
    insertJobsInQueue(readyQueue);

    int counter = 0;
    vector<Job> finishedJobs;

    
    int totalWaitingTime = 0;
    // process jobs while there are jobs available in ready queue
    while(!readyQueue.empty()){
        Job curJob = readyQueue.front(); readyQueue.pop();

        curJob.waitingTime = (curJob.arrivalTime > counter) ? 0 : (counter - curJob.arrivalTime);

        totalWaitingTime += curJob.waitingTime;

        curJob.completionTime = curJob.arrivalTime + curJob.waitingTime + curJob.burstTime;
        counter = curJob.completionTime;

        // since the job is completed, hence we can add the job in the finishedJobs array
        finishedJobs.push_back(curJob);
    }

    // print the job table 

    cout << setw(20) << "Name" << setw(5) << "A.T" << setw(5) << "B.T" << setw(5) << "W.T" << setw(5) << "C.T" << endl;
    for(auto job : finishedJobs){
        job.printJobStatistics();
    }

    cout << "Total time took to complete all the jobs in the ready queue = " << counter << " seconds" << endl;
    cout << "Average waiting time = " << double(totalWaitingTime / nameOfJobs.size()) << endl;

}


