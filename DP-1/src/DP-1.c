#include "../../common/inc/shared_mem.h"

int main(int argc, char *argv[]) {

    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    attach_shared_mem(&shm, shm_id);

    // Initialize semaphore
    sem = sem_open("/sem", O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        fprintf(stderr, "Failed to create semaphore: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory
    shm->write_index = 0;
    shm->read_index = 0;
    memset(shm->buffer, '\0', BUFFER_SIZE);

    // Fork DP-2 process
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Failed to fork DP-2 process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process (DP-2)
        char shm_id_str[10];
        char dp1_pid_str[10];

        sprintf(shm_id_str, "%d", shm_id);      // Shared Memory ID
        sprintf(dp1_pid_str, "%d", getpid());   //DP-1 PID

        execl("./DP-2", "DP-2", shm_id_str, dp1_pid_str, NULL);
        
        fprintf(stderr, "Failed to launch DP-2 process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Register signal handler for SIGINT
    signal(SIGINT, handle_sigint_dp);

    // Main loop
    while (1) {
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

    return 0;
}
