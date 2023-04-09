#include "../../common/inc/shared_mem.h"

sem_t *sem = NULL;
shared_mem_t *shm = NULL;


void handle_sigint(int sig)
{
    detach_shared_mem(shm);
    // sem_close(sem);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int shm_id = 0;
    
    int pipe_fds[2];
    pipe(pipe_fds);
    char buf[1];
    
    /* When DP-1 is launched, it will allocate enough shared memory to
    hold a circular buffer of 256 characters, plus enough space for the
    2 indices (i.e. the read index and the write index) */

    /* DP-1 is alone responsible for the generation
    and creation of the shared memory key */
    if (init_shared_mem(&shm_id) == EXIT_FAILURE)
    {
        //perror(stderr);
        return EXIT_FAILURE;
    }
    printf("[DP-1] Memory id %d will be used\n", shm_id);
    
    /* After the shared memory has been created, DP-1
    will attach to it */
    if (attach_shared_mem(&shm, shm_id) == EXIT_FAILURE)
    {
        //perror(stderr);
        return EXIT_FAILURE;
    }
    printf("[DP-1] Attached memory successfully\n");

    /* this would be a good time to also
    initialize the shared memory elements ... */
    shm->write_index = 0;
    shm->read_index = 0;
    memset(shm->buffer, '\0', BUFFER_SIZE);
    printf("[DP-1] Initialized element in shared memory\n");

    /* DP-1 will launch DP-2 (through the use of a fork() call)
    in the launching of DP-2, only the sharedMemoryID (shmID) will
    be passed as a command line argument to the DP-2 process*/
    pid_t dp_2_fork = fork();
    if (dp_2_fork == -1)
    {
        printf("[DP-1] Error - The fork() call failed!\n");
        exit(EXIT_FAILURE);
    }
    if (dp_2_fork == 0)
    {
        pid_t procID = getpid();
        pid_t pProcID = getppid();

        char theArg[25] = "";
        sprintf(theArg,"%d", shm_id);

        printf ("[DP-1] child my PID is          - %ld\n", (long)procID);
        printf ("[DP-1] child my Parent's PID is - %ld\n", (long)pProcID);

        printf("[DP-1] child I am in the child!!\n");
        read (pipe_fds[0], buf, 1);
        // Child process (DP-2)
        if(execl("../../DP-2/bin/DP-2", "DP-2", theArg, NULL) == -1)
        {
            printf("[DP-1] child the execl failed\n");
        }
    }
    else
    {
        pid_t procID = getpid();
        pid_t pProcID = getppid();

        printf ("[DP-1] parent my PID is          - %ld\n", (long)procID);
        printf ("[DP-1] parent my Parent's PID is - %ld\n", (long)pProcID);

        write(pipe_fds[1], "_", 1);

        sleep(30);

        printf("[DP-1] I'm done my \"work\" - I'm leaving ...\n");
        exit (EXIT_SUCCESS);


        /* // Register signal handler for SIGINT
        signal(SIGINT, handle_sigint);

        // Main loop
        while (1) {
            // Generate 20 random letters
            char letters[20];
            srand(time(NULL));
            for (int i = 0; i < 20; i++) {
                letters[i] = 'A' + (rand() % 20);
            }

            // Write letters to shared memory
            //sem_wait(sem);
            int space_available = BUFFER_SIZE - shm->write_index + shm->read_index;
            int num_to_write = space_available >= 20 ? 20 : space_available;
            for (int i = 0; i < num_to_write; i++) {
                shm->buffer[shm->write_index] = letters[i];
                shm->write_index = (shm->write_index + 1) % BUFFER_SIZE;
            }
            //sem_post(sem);

            // Sleep for 2 seconds
            sleep(2);
        } */
    }
    

        
    
    return 0;
}
