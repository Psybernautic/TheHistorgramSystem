#include "../inc/shared_mem.h"

/* 
 *  FUNCTION    : int init_shared_mem(int *shm_id)
 *  DESCRIPTION :
 *  This function initializes a shared memory segment and returns its ID to the calling function.
 *  The function creates a new shared memory segment using shmget(), sets the size of the segment to the size of a shared_mem_t structure, and grants read and write permissions to the segment using IPC_CREAT | 0666.
 *  If the shared memory segment creation fails, the function prints an error message to stderr and returns -1.
 *  PARAMETERS  : int *shm_id: a pointer to an integer variable where the shared memory ID will be stored.
 *  RETURNS     : 0 if the shared memory segment is successfully created.
 *               -1 if the shared memory segment creation fails.
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
 *  FUNCTION    : attach_shared_mem(shared_mem_t **shm_ptr, int shm_id)
 *  DESCRIPTION :
 *  This function attaches the calling process to an existing shared memory segment.
    The function uses shmat() to attach to the shared memory segment with the specified ID and sets the value of the shm_ptr pointer to the address of the attached memory segment.
 *  PARAMETERS  : shared_mem_t **shm_ptr: a double pointer to a shared_mem_t struct where the address of the attached shared memory segment will be stored.
 *                int shm_id: the ID of the shared memory segment to attach to.
 *  RETURNS     : None
 */
void attach_shared_mem(shared_mem_t **shm_ptr, int shm_id) {
    *shm_ptr = shmat(shm_id, NULL, 0);
    if (*shm_ptr == (void *) -1) {
        fprintf(stderr, "Failed to attach to shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/* 
 *  FUNCTION    : detach_shared_mem(shared_mem_t *shm_ptr)
 *  DESCRIPTION :
 *  This function detaches the calling process from a shared memory segment that was previously attached.
 *  The function uses shmdt() to detach from the shared memory segment with the specified address.
 *  PARAMETERS  : sshared_mem_t *shm_ptr: a pointer to the shared memory segment to detach from.           
 *  RETURNS     : None
 */
void detach_shared_mem(shared_mem_t *shm_ptr) {
    if (shmdt(shm_ptr) < 0) {
        fprintf(stderr, "Failed to detach from shared memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/* 
 *  FUNCTION    : handle_sigint_dp(int sig)
 *  DESCRIPTION :
 *  This function is the signal handler for the SIGINT signal in the DP process.
 *  The function detaches the shared memory segment and closes the semaphore before terminating the process with a success exit code.
 *  PARAMETERS  : int sig: the signal that triggered the signal handler.        
 *  RETURNS     : None
 */
void handle_sigint_dp(int sig) {
    detach_shared_mem(shm);
    sem_close(sem);
    exit(EXIT_SUCCESS);
}