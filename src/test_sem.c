#include "sem.h"

/*
    Really bad semaphore test code where a N_PROC processes go and try to grab the semaphore.
    If my_procnum < 10, then the processes wait patiently for resources. If my_procnum >=10, then
    the processes try to get the resources. If they fail, they broadcast it to stdout and give up.

    Whenever a process gets a resource, it sleeps while holding the resource for 0.1 ms because
    they like the resources. Then they return it with sem_inc.
*/

#define NUM_RESOURCES 5
int main() {
    struct sem *semaphore = mmap(NULL, sizeof(struct sem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    // create delicious scarce semaphore resources
    sem_init(semaphore, NUM_RESOURCES);
    
    int child_pid = 1;
    
    // fork a N_PROC processes
    for(int i = 0; i < N_PROC; i++) {
        if(child_pid == 0) {
            continue;
        }
        child_pid = fork();
        if(child_pid == 0) {
            my_procnum = i + 1; 
        }
    }
    
    if(child_pid != 0) {
        my_procnum = 0; 
    }
        
    // every child process with my_procnum < 10 waits for resources
    if ((child_pid == 0) && (my_procnum < 10)){
        sem_wait(semaphore); // every process tries to get a semaphore
        printf("Process with my_procnum %d got semaphore\n", my_procnum);
        usleep(100000); // process hoards semaphore for 1000000 microseconds (0.1 ms)
        sem_inc(semaphore);
        printf("Process with my_procnum %d returned semaphore\n", my_procnum);
    }

    // every child process with my_procnum >= 10 trys to get resource
    if ((child_pid == 0) && (my_procnum >= 10)) {
        if (sem_try(semaphore) == 0) {
            printf("Process with my_procnum %d tried and got blocked\n", my_procnum);
        } else {
            printf("Process with my_procnum %d tried and got semaphore\n", my_procnum);
            usleep(100000); // process hoards semaphore for 1000000 microseconds (0.1 ms)
            sem_inc(semaphore); // return the semaphore
            printf("Process with my_procnum %d returned semaphore\n", my_procnum);
            
        }
    }


    // children exit here
    if(child_pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // parent waits for all children
    while(wait(NULL) > 0);
    
    // // Parent prints result
    // if(child_pid != 0) {
    //     fprintf(stdout, "Counter: %lld (expected: 10000000)\n", *counter);
    //     if(*counter == 10000000) {
    //         fprintf(stdout, "SUCCESS!\n");
    //     } else {
    //         fprintf(stdout, "FAILURE - race condition detected\n");
    //     }
    // }
    
    return 0;
}