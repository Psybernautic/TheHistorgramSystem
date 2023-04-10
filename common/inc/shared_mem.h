#ifndef _SHARED_MEM_H_
#define _SHARED_MEM_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define SMALL_BUFFER 15
#define BUFFER_SIZE 258
#define SHM_SIZE 256
#define SHM_KEY 12345
#define NUM_LETTERS 20

// Shared structure amongst the Data Producers

typedef struct {
    char buffer[BUFFER_SIZE];
    int write_index;
    int read_index;
} shared_mem_t;

// global variables

extern sem_t *sem;
//extern shared_mem_t *shm;

// Function Prototypes

int init_shared_mem(int *shm_id);
int attach_shared_mem(shared_mem_t **shm_ptr, int shm_id);
void detach_shared_mem(shared_mem_t *shm_ptr);
void handle_sigint_dp(int sig);



#endif /*_SHARED_MEM_H*/