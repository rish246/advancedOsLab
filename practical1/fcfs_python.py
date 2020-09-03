# create a class named Job
class Job:

    def __init__(self, name, arrival_time, burst_time):
        self.name = name
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.waiting_time = 0
        self.completion_time = 0

    def print_stats(self):
        print(self.name, self.arrival_time, self.burst_time, self.waiting_time, self.completion_time)


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




def main():
    # information of the jobs
    names_of_jobs = ["MsWord", "Vscode", "Notepad", "Intellij", "Google Chrome"]
    arrival_times = [0, 1, 2, 3, 4]
    burst_times = [2, 6, 4, 9, 12]

    # initialize the ready queue
    ready_queue = []
    ready_queue = insert_jobs_in_queue(ready_queue, names_of_jobs, arrival_times, burst_times)

    # store the finished jobs
    finished_jobs = []

    booked_slot = 0
    total_waiting_time = 0

    
    for job in ready_queue:
        
        # update the values of waiting times and completion time
        job.waiting_time = booked_slot < job.arrival_time and 0 or (booked_slot - job.arrival_time)

        total_waiting_time += job.waiting_time

        job.completion_time = job.arrival_time + job.waiting_time + job.burst_time

        booked_slot = job.completion_time

        finished_jobs.append(job)

    
    average_waiting_time = (total_waiting_time / len(names_of_jobs))

    print(f'Name A.T B.T W.T C.T')
    for job in finished_jobs:
        job.print_stats()


    print(f'average waiting time in the system  = {average_waiting_time}')
    print(f'total time took to complete all the jobs = {booked_slot}')


main()

