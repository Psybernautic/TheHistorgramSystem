#include "../inc/shared_mem.h"

#define COMMON_KEY 16535

/* 
 * Initializes a new shared memory segment with the size of shared_mem_t and 
 * sets the ID of the segment in the pointer passed as argument.
 * Returns 0 on success, -1 on failure.
 */
int init_shared_mem(int *shm_id)
{
    /* DP-1 will follow best practices and check for the
    existence of the shared memory, and if not found, will
    create it. In this respect then, DP-1 is alone
    responsible for the generation and creation of the shared
    memory key */

    /* Getting the unique key for the shared memory */
	key_t theKey = ftok(".", COMMON_KEY);
	if (theKey == -1)
	{
        fprintf(stderr, "Failed to create key: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

    /* checking if the shared memory exists */
	*shm_id = shmget(theKey, sizeof(shared_mem_t), 0);
    if (*shm_id == -1)
    {
        /* Creating the shared memory segment */
		if ((*shm_id = shmget(theKey, sizeof(shared_mem_t), IPC_CREAT | 0666)) == -1)
		{
            fprintf(stderr, "Failed to create memory: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS; // Created new shared memory
    }
    return EXIT_SUCCESS; // Using existing shared memory
}

/*
 * Attaches to an existing shared memory segment with the specified ID
 * and sets the pointer passed as argument to the attached memory address.
 * Exits with failure status if attachment fails.
 */
int attach_shared_mem(shared_mem_t **shm_ptr, int shm_id)
{
    /* Attaching the memory to our data space */
    *shm_ptr = (shared_mem_t *)shmat(shm_id, NULL, 0);
    if (*shm_ptr == (shared_mem_t *)-1)
    {
        fprintf(stderr, "Failed to attach to shared memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
}

/*
 * Detaches from the specified shared memory segment.
 * Exits with failure status if detachment fails.
 */
void detach_shared_mem(shared_mem_t *shm_ptr)
{
    if (shmdt(shm_ptr) == -1)
    {
        fprintf(stderr, "Failed to detach from shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void handle_sigint_dp(int sig) {
    detach_shared_mem(shm);
    sem_close(sem);
    exit(EXIT_SUCCESS);
}