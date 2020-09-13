#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<string> namesOfProcesses = {"P1", "P2", "P3", "P4", "p5"};
vector<int> arrivalTimes = {0, 1, 2, 3, 4};
vector<int> burstTimes = {2, 6, 4, 9, 12};


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

void insertJobsInJobQueue(vector<Job> &jobQueue);

void insertJobsInReadyQueue(queue<Job*> &readyQueue, vector<Job> &jobQueue, int timer);

void printFinalInformation(vector<Job> &finishedJobs, int totalWaitingTime, int totalTurnAroundTime);


class RoundRobinScheduler {
public:



    void scheduleJobs(vector<Job> jobQueue) {

        sort(jobQueue.begin(), jobQueue.end(), onArrivalTime);


        vector<Job> finishedJobs = {};
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


    static bool onArrivalTime(Job j1, Job j2){
        return j1.arrivalTime < j2.arrivalTime;
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



};

class SjfScheduler {
    protected:

        const int inf = 100;
        vector<Job> finishedJobs = {};
        int totalWaitingTime = 0, totalTurnAroundTime = 0;


        void insertJobsInReadyQueue(vector<Job*> &readyQueue, vector<Job> &jobQueue, int timer){

        // check for all the jobs which are arrived before timer
        // passed in original jobQueue
            for(auto &job : jobQueue){
                // check if job has arrived and has not been served yed
                if(job.arrivalTime <= timer && job.isServed == false){
                    Job* jobptr = &job;
                    // push address of job in the ready queue
                    readyQueue.push_back(jobptr);
                }
            }

        }

        static bool onBurstTime(Job* j1, Job* j2){
            return j1->burstTime <= j2->burstTime;
        }
};

class SjfPreemptiveScheduler : public SjfScheduler{
    

    public:
        void scheduleJobs(vector<Job> jobQueue){
            int timer = 0;
            

            while(timer < inf){
                // address of all job objects
                vector<Job*> readyQueue;

                // this code is wrong
                insertJobsInReadyQueue(readyQueue, jobQueue, timer);


                // if no job has arrived, then don't do anything
                if(readyQueue.empty()){
                    timer++;
                    continue;
                } 

                // sort based on birst time
                sort(readyQueue.begin(), readyQueue.end(), onBurstTime);

                // // get the first job
                // the first job is the job with min burst time
                // select the job for processing
                Job* jobToServe = readyQueue[0]; // address of the job to serve

                // provide service to the selected job
                // serveJob(jobToServe, timer);
                

                // increase the timer to the completion time value
                // cpu will be busy till the current job is served
                // so till then, add the arriving jobs in readyQueue
                jobToServe->burstTime -= 1;

                // cout << jobToServe->name << ' ' << timer << ' ' << jobToServe->burstTime << ' ' << endl;
                // correct serveJobs function



                // once processed, add the job to finished jobs array
                if(jobToServe->burstTime == 0) {
                    serveJob(jobToServe, timer);
                    totalWaitingTime += jobToServe->waitingTime;
                    totalTurnAroundTime += jobToServe->turnAroundTime;

                    
                    finishedJobs.push_back(*jobToServe);
                }

                timer ++;

                




            }
            


            // print all the necessary information like:
            //      1. Table of information
            //      2. Average waiting time
            printFinalInformation(finishedJobs, totalWaitingTime, totalTurnAroundTime);
        }

        // function that will serve the job
        // calculate the average waiting time
        // completion time
        // response time for that job
        void serveJob(Job* jobToServe, int timer){
            jobToServe->isServed = true;
            jobToServe->completionTime = timer + 1;
            jobToServe->turnAroundTime = jobToServe->completionTime - jobToServe->arrivalTime;
            jobToServe->waitingTime = jobToServe->turnAroundTime - jobToServe->originalBurstTime;
            // jobToServe->responseTime = jobToServe->waitingTime; 
        }
    


};




class SjfNonPreemptiveScheduler : public SjfScheduler{

    public:
        void scheduleJobs(vector<Job> jobQueue){
            int timer = 0;


            while(timer < inf){
                // address of all job objects
                vector<Job*> readyQueue;

                // this code is wrong
                insertJobsInReadyQueue(readyQueue, jobQueue, timer);

                // if no job has arrived, then don't do anything
                if(readyQueue.empty()){
                    timer++;
                    continue;
                } 

                // sort based on birst time
                sort(readyQueue.begin(), readyQueue.end(), onBurstTime);

                // // get the first job
                // the first job is the job with min burst time
                // select the job for processing
                Job* jobToServe = readyQueue[0]; // address of the job to serve

                // provide service to the selected job
                serveJob(jobToServe, timer);

                // increase the timer to the completion time value
                // cpu will be busy till the current job is served
                // so till then, add the arriving jobs in readyQueue
                timer = jobToServe->completionTime;


                totalWaitingTime += jobToServe->waitingTime;

                totalTurnAroundTime += jobToServe->turnAroundTime;


                // once processed, add the job to finished jobs array
                finishedJobs.push_back(*jobToServe);

                




            }
            


            // print all the necessary information like:
            //      1. Table of information
            //      2. Average waiting time
            printFinalInformation(finishedJobs, totalWaitingTime, totalTurnAroundTime);
        }


        void serveJob(Job* jobToServe, int timer)
        {
            jobToServe->isServed = true;

            jobToServe->waitingTime = timer - jobToServe->arrivalTime;

            jobToServe->completionTime = timer + jobToServe->burstTime;

            jobToServe->turnAroundTime = jobToServe->completionTime - jobToServe->arrivalTime;
        }


};




class FcfsScheduler{
    public:
        void scheduleJobs(vector<Job> jobQueue){
            int timer = 0;

    // stores the finished jobs
            vector<Job> finishedJobs;

            // calculate the total waiting time of the processes
            int totalWaitingTime = 0, totalTurnAroundTime = 0;

            // process jobs while there are jobs available in ready queue
            // delay of counter time 
            while(!jobQueue.empty()){
                // get the first job from the readyQueue
                Job jobToServe = jobQueue[0]; jobQueue.erase(jobQueue.begin());

                // get the time at which job will leave the system
                serveJob(jobToServe, timer);

                totalTurnAroundTime += jobToServe.turnAroundTime;


                totalWaitingTime += jobToServe.waitingTime;

                // book a new time slot for the new job
                timer = jobToServe.completionTime;

                // since the job is completed, hence we can add the job in the finishedJobs array
                finishedJobs.push_back(jobToServe);
            }

            printFinalInformation(finishedJobs, totalWaitingTime, totalTurnAroundTime);
        }


        void serveJob(Job &jobToServe, int timer){
            
            jobToServe.waitingTime = (jobToServe.arrivalTime > timer) ? 0 : (timer - jobToServe.arrivalTime);

            jobToServe.completionTime = jobToServe.arrivalTime + jobToServe.waitingTime + jobToServe.burstTime;

            jobToServe.turnAroundTime = jobToServe.completionTime - jobToServe.arrivalTime;


            

        }
};




int main()
{
    vector<Job> jobQueue = {};

    insertJobsInJobQueue(jobQueue);

    // start the processing

    cout << "Round Robin Scheduler ..............." << endl;
    RoundRobinScheduler roundRobinScheduler;
    roundRobinScheduler.scheduleJobs(jobQueue);

    cout << "Sjf Preemptive Scheduler ..............." << endl;
    SjfPreemptiveScheduler sjfPreemptiveScheduler;
    sjfPreemptiveScheduler.scheduleJobs(jobQueue);

    cout << "Sjf NonPreemptive Scheduler ..............." << endl;
    SjfNonPreemptiveScheduler sjfNonPreemptiveScheduler;
    sjfNonPreemptiveScheduler.scheduleJobs(jobQueue);

    cout << "fcfc scheduler ..............." << endl;
    FcfsScheduler fcfsScheduler;
    fcfsScheduler.scheduleJobs(jobQueue);


}


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


// why am i getting waiting time == (turnAroundTime)

// higher average waiting time in round_robin.cpp