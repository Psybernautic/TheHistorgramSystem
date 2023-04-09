#ifndef _SHARED_MEM_H_
#define _SHARED_MEM_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 256

// Shared structure amongst the Data Producers

typedef struct {
    char buffer[BUFFER_SIZE];
    int write_index;
    int read_index;
} shared_mem_t;

// Function Prototypes

int init_shared_mem(int *shm_id);
void attach_shared_mem(shared_mem_t **shm_ptr, int shm_id);
void detach_shared_mem(shared_mem_t *shm_ptr);



#endif /*_SHARED_MEM_H*/