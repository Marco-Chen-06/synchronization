#include "fifo.h"

int main(int argc, char* argv[]) {
    struct myfifo *fifo = mmap(NULL, sizeof(struct myfifo) + sizeof(struct sem) * 3, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);    
    fifo_init(fifo);
    int num_writers = 0; // number of writers
    int num_items_each = 0; // number of items each writer wants to write

    int opt;
    while ((opt = getopt(argc, argv, "w:n:")) != -1) {
        switch (opt) {
            case 'w':
                num_writers = atoi(optarg);
                if (num_writers > 62) {
                    fprintf(stderr, "Error, can't have more than 62 writers, sorry. Semaphore can't fit that many.\n");
                    return -1;
                }
                break;
            case 'n':
                num_items_each = atoi(optarg); 
                break;
            case '?':
                return -1;
        }
    }

    int total_items_to_write = num_writers * num_items_each; // total items to write
    int num_items_read_by_reader = 0; // only used to keep track of how many times the reader called fifo_rd

    int pid = 1; // make sure parent doesnt get mistaken as a child by our weird test code
    my_procnum = 0;
    // fork a num_writers processes and one extra reader process
    for(int i = 0; i < num_writers + 1; i++) {
        if(pid == 0) {
            continue;
        }
        pid = fork();
        if(pid == 0) {
            my_procnum = i + 1; 
        }
    }

    // at this point, parent has my_procnum 0, and we have num_writers children
    // (i will treat the child with my_procnum 1 as the reader)

    // only parent runs this code
    if (pid == 1) {
    fprintf(stdout, "Beginning test with %d writers, %d items each\n", num_writers, num_items_each);
    }

    // only writer children run this code
    if ((pid == 0) && (my_procnum > 1)) {
        for (int i = 0; i < num_items_each; i++) {
            fifo_wr(fifo, i);
            // printf("Writer %d wrote a %d to the fifo.\n", my_procnum - 1, i);
        }
        printf("Writer %d stream completed\n", my_procnum-1);
    }

    // reader process tries to read num_writers * num_items_each items from the fifo
    if ((pid == 0) && (my_procnum == 1)) {
        for (int i = 0; i < total_items_to_write; i++) {
            // fifo_rd(fifo);
            printf("Reader read a %d\n ", fifo_rd(fifo));
            num_items_read_by_reader++;
        }
        printf("Reader successfully called fifo_rd %d times\n", num_items_read_by_reader);
    }

    // children exit here
    if(pid == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // parent waits for all children
    while(wait(NULL) > 0);

    printf("All children collected\n");
    return 0;
}

