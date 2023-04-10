#include "../../common/inc/shared_mem.h"

shared_mem_t *shm = NULL;

void handle_sigint_dp(int sig) {
    detach_shared_mem(shm);
    sem_close(sem);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    /* In the launching of DP-2, only the sharedMemoryID (shmID)
    will be passed as a command line argument to the DP-2 process */
    int shm_id = atoi(argv[1]);

    /* After being launched, DP-2 will: */

    pid_t procID = getpid();    /* Need to get the processID (PID) of itself */
    pid_t pProcID = getppid();  /* and the processID (PID) of DP-1 (it’s parent) */
    printf("[DP-2] my PID is          - %ld\n", (long)procID);
    printf("[DP-2] my Parent's PID is - %ld\n", (long)pProcID);
    printf("[DP-2] The shmem id is %s\n", argv[1]);
    
    /* At this point, DP-2 will launch the DC application (through the
    use of a fork() call) and pass it’s PID, it’s parent’s (DP-1) PID
    as well as the sharedMemoryID (shmID) as command line arguments
    into the DC application */
    char dp2_pid_str[SMALL_BUFFER] = "";
    char dp2_ppid_str[SMALL_BUFFER] = "";
    char shmID_arg[SMALL_BUFFER] = "";
    sprintf(dp2_pid_str, "%d", procID);     // Getting Its own PID as an argument
    sprintf(dp2_ppid_str, "%d", pProcID);   // Getting its parent's PID as an argument
    sprintf(shmID_arg, "%d", shm_id);       // Getting the shared memory id as an argument
    
    // Fork DC process
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "[DP-2] Failed to fork DC process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process (DC)
        if(execl("../../DC/bin/DC", "DC", dp2_pid_str, dp2_ppid_str, shmID_arg, NULL) == -1)
        {
            printf("[DP-2] child the execl failed\n");
        }
    }
    else
    {
        /* Only after forking and launching the DC application will DP-2
        attach itself to the block of shared memory */
        if (attach_shared_mem(&shm, shm_id) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        // Register signal handler for SIGINT
        signal(SIGINT, handle_sigint_dp);

        /* After their initial setup (detailed above) – both the DP-1 and DP-2
        process enter their main loop. Both processes begin to generate their
        random letters and write the information into the circular buffer */
        while (1)
        {
            // Generate 20 random letters
            char letters[20];
            srand(time(NULL));
            for (int i = 0; i < 20; i++) {
                letters[i] = 'A' + (rand() % 20);
            }

            // Write letters to shared memory
            sem_wait(sem);
            int space_available = BUFFER_SIZE - shm->write_index + shm->read_index;
            int num_to_write = space_available >= 20 ? 20 : space_available;
            for (int i = 0; i < num_to_write; i++) {
                shm->buffer[shm->write_index] = letters[i];
                shm->write_index = (shm->write_index + 1) % BUFFER_SIZE;
            }
            sem_post(sem);

            // Sleep for 2 seconds
            sleep(2);
        }
    }

    return 0;
}
