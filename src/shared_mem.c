#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include "../inc/shared_mem.h"

/* 
 * Initializes a new shared memory segment with the size of shared_mem_t and 
 * sets the ID of the segment in the pointer passed as argument.
 * Returns 0 on success, -1 on failure.
 */
int init_shared_mem(int *shm_id) {
    *shm_id = shmget(IPC_PRIVATE, sizeof(shared_mem_t), IPC_CREAT | 0666);
    if (*shm_id < 0) {
        fprintf(stderr, "Failed to create shared memory: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/*
 * Attaches to an existing shared memory segment with the specified ID
 * and sets the pointer passed as argument to the attached memory address.
 * Exits with failure status if attachment fails.
 */
void attach_shared_mem(shared_mem_t **shm_ptr, int shm_id) {
    *shm_ptr = shmat(shm_id, NULL, 0);
    if (*shm_ptr == (void *) -1) {
        fprintf(stderr, "Failed to attach to shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/*
 * Detaches from the specified shared memory segment.
 * Exits with failure status if detachment fails.
 */
void detach_shared_mem(shared_mem_t *shm_ptr) {
    if (shmdt(shm_ptr) < 0) {
        fprintf(stderr, "Failed to detach from shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
