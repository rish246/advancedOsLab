class Job:
    def __init__(self, name, arrival_time, burst_time):
        self.name = name
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.completion_time = 0
        self.waiting_time = 0
        self.response_time = 0
        self.served = False

    def print_stats(self):
        print(f'{self.name.center(10)}\t|\t{self.arrival_time}\t|\t{self.burst_time}\t|\t{self.waiting_time}\t|\t{self.completion_time}')
        

def insert_jobs_in_job_queue(job_queue, names, arrival_times, burst_times):

    number_of_jobs = len(names)

    for i in range(number_of_jobs):
        name_of_cur_job = names[i]

        arrival_time_of_current_job = arrival_times[i]

        burst_time_of_cur_job = burst_times[i]

        newJob = Job(name_of_cur_job, arrival_time_of_current_job, burst_time_of_cur_job)

        job_queue.append(newJob)

    # make a pass by referece call

def insert_jobs_in_ready_queue(ready_queue, job_queue, timer):
    for job in job_queue:
        if job.arrival_time <= timer and job.served == False:
            ready_queue.append(job)

def by_burst_time(first_job, second_job):
    return first_job.burst_time - second_job.burst_time


def serve_job(job, timer):
    job.served = True

    job.waiting_time = timer - job.arrival_time

    job.completion_time = timer + job.burst_time

    job.response_time = job.waiting_time

def print_final_information(finished_jobs, total_waiting_time):

    average_waiting_time = total_waiting_time / len(finished_jobs)
    print('----------------------------------------------------------------------------')    
    print(f'  Name\t\t\tA.T\t\tB.T\t\tW.T\t\tC.T')
    print('----------------------------------------------------------------------------')
    for job in finished_jobs:
        job.print_stats()
    print('----------------------------------------------------------------------------')

    print(f'average waiting time in the system  = {average_waiting_time}')


def main():

    names = ["P1", "P2", "P3", "P4", "P5"]
    arrival_times = [2, 5, 1, 0, 4]
    burst_times = [6, 2, 8, 3, 4]

    job_queue = []
    finished_jobs = []

    insert_jobs_in_job_queue(job_queue, names, arrival_times, burst_times)

    timer = 0
    total_waiting_time = 0
    while(timer < 100):
        ready_queue = []

        insert_jobs_in_ready_queue(ready_queue, job_queue, timer)
    
        if len(ready_queue) == 0:
            timer += 1
            continue

        ready_queue.sort(key = lambda x : x.burst_time)


        best_job_to_serve = ready_queue[0]

        serve_job(best_job_to_serve, timer) 

        timer = best_job_to_serve.completion_time

        total_waiting_time += best_job_to_serve.waiting_time

        finished_jobs.append(best_job_to_serve)


    print_final_information(finished_jobs, total_waiting_time)
    

    # make the job queue
main()