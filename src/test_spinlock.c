#include "sem.h"
#include <sys/mman.h>
#include <sys/wait.h>

int main() {
    // Create shared memory for semaphore and counter
    struct sem *semaphore = mmap(NULL, sizeof(struct sem), 
                                 PROT_READ | PROT_WRITE, 
                                 MAP_SHARED | MAP_ANON, -1, 0);
    
    long long int *counter = mmap(NULL, sizeof(long long int), 
                                  PROT_READ | PROT_WRITE, 
                                  MAP_SHARED | MAP_ANON, -1, 0);
    
    *counter = 0;
    
    // Initialize semaphore with 1 (acts like a mutex)
    sem_init(semaphore, 1);
    
    int child_pid = 1;
    
    // Fork 9 times to create 10 processes total
    for(int i = 0; i < 9; i++) {
        if(child_pid == 0) {
            continue;
        }
        child_pid = fork();
        if(child_pid == 0) {
            my_procnum = i + 1;  // Set process number for children
        }
    }
    
    if(child_pid != 0) {
        my_procnum = 0;  // Parent is process 0
    }
    
    // Each process increments counter 1 million times
    sem_wait(semaphore);
    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            *counter += 1;
        }
    }
    sem_inc(semaphore);
    
    // Children exit here
    if(child_pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // Parent waits for all children
    while(wait(NULL) > 0);
    
    // Parent prints result
    if(child_pid != 0) {
        fprintf(stdout, "Counter: %lld (expected: 10000000)\n", *counter);
        if(*counter == 10000000) {
            fprintf(stdout, "SUCCESS!\n");
        } else {
            fprintf(stdout, "FAILURE - race condition detected\n");
        }
    }
    
    return 0;
}