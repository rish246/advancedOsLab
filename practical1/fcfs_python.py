##############################################################################################################################################
# create a class named Job
class Job:

    def __init__(self, name, arrival_time, burst_time):
        self.name = name
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.waiting_time = 0
        self.completion_time = 0

    def print_stats(self):
        TAB = "\t\t"
        print(f'{self.name.center(10)}\t|\t{self.arrival_time}\t|\t{self.burst_time}\t|\t{self.waiting_time}\t|\t{self.completion_time}')
################################################################################################################################################
##############################################################################################
# function to insert jobs in queue
def insert_jobs_in_queue(ready_queue, names_of_jobs, arrival_times, burst_times):
    number_of_jobs = len(names_of_jobs)

    ## take each name, arrival time and burst time
    # create a Job object 
    # push object to ready_queue
    for i in range(number_of_jobs):
        name_of_cur_job = names_of_jobs[i]
        arrival_time_of_cur_job = arrival_times[i]
        burst_time_of_cur_job = burst_times[i]

        ## create a job object
        new_job = Job(name_of_cur_job, arrival_time_of_cur_job, burst_time_of_cur_job)
        ready_queue.append(new_job)

    return ready_queue
#################################################################################################


##################################################################################################
# function that will process each job in the ready queue 
# returns queue of finished jobs(in particular order)
# total_processing_time => time it took to process all the jobs in ready queue
# total_waiting_time => total waiting time of the system
def process_jobs(ready_queue):
    finished_jobs = []

    total_processing_time = 0
    total_waiting_time = 0


    # take each job from ready queue
    # set its waiting time and completion time values
    
    for job in ready_queue:
        
        # update the values of waiting times and completion time
        job.waiting_time = total_processing_time < job.arrival_time and 0 or (total_processing_time - job.arrival_time)

        total_waiting_time += job.waiting_time

        job.completion_time = job.arrival_time + job.waiting_time + job.burst_time

        total_processing_time = job.completion_time

        finished_jobs.append(job)

    
    return finished_jobs, total_processing_time, total_waiting_time
########################################################################################################

#####################################################################################
# function that will print the table and final information related to the algorithm

def print_final_information(finished_jobs, average_waiting_time, total_processing_time):
    print('----------------------------------------------------------------------------')    
    print(f'  Name\t\t\tA.T\t\tB.T\t\tW.T\t\tC.T')
    print('----------------------------------------------------------------------------')
    for job in finished_jobs:
        job.print_stats()
    print('----------------------------------------------------------------------------')

    print(f'average waiting time in the system  = {average_waiting_time}')
    print(f'total time took to complete all the jobs = {total_processing_time}')
########################################################################################



########################################################################################
############################ main function ############################################# 
def main():
    # information of the jobs
    names_of_jobs = ["MsWord", "Vscode", "Notepad", "Intellij", "Google Chrome"]
    arrival_times = [0, 1, 2, 3, 4]
    burst_times = [2, 6, 4, 9, 12]

    # initialize the ready queue
    ready_queue = []
    ready_queue = insert_jobs_in_queue(ready_queue, names_of_jobs, arrival_times, burst_times)

    # store the finished jobs
    
    finished_jobs, total_processing_time, total_waiting_time = process_jobs(ready_queue)
    
    average_waiting_time = (total_waiting_time / len(names_of_jobs))

    print_final_information(finished_jobs, average_waiting_time, total_processing_time)
    
###############################################################################################

main()

