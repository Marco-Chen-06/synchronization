#include "spinlock.h"
int main() {

    //use mmap here to make shared memory region
    //use MAP_SHARED
    //use 

    long long int *counter = mmap(NULL, 4096, PROT_READ| PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    volatile char *lock = mmap(NULL, 4096, PROT_READ| PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    *counter = 0;

    int child_pid = 1;

    for(int i = 0; i < 9; i++) { //my macOS has 10 cores total
        if(child_pid == 0) {
            continue;
        }
        child_pid = fork();
    } 

    /*
        With the spin lock, we can make sure each child waits its turn and increments counter 1 million times

        I notice that without the spin lock, we get random numbers for counter. This is for two main reasons: 
        1. The parent prints before all the children can increment counter 1 million times.
        2. All the children are incrementing counter, but *counter += 1 isn't even atomic so even if the parent
        printed after all the children exited, we still probably wouldnt get 10 million
    */
    spin_lock(lock);
    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            *counter += 1;
        } 
    }
    spin_unlock(lock);

    // children exit here
    if(child_pid == 0) {
        exit(EXIT_SUCCESS);
    }

    while(wait(NULL) > 0); //wait for all child processes to die

    if(child_pid != 0) {
        fprintf(stdout, "%lld\n", *counter); //print out result
    }

}
