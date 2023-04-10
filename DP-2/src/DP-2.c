#include "../../common/inc/shared_mem.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [shm_id]\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    // PID's of both processes

    // Parent PID
    char dp1_pid_str[10];
    sprintf(dp1_pid_str, "%d", getppid());

    // DP-2 PID
    char dp2_pid_str[10];
    sprintf(dp2_pid_str, "%d", getpid());   //DP-2 PID

    int shm_id = atoi(argv[1]);             // Shared Memory ID
    attach_shared_mem(&shm, shm_id);

    // Fork DC process
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Failed to fork DC process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process (DC)
        char *args[] = {"../../DC/bin/DC", argv[1], dp1_pid_str, dp2_pid_str, NULL};
        execvp(args[0], args);
        fprintf(stderr, "Failed to launch DC process: %s\n", strerror(errno));
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

        // Calculate the amount of space available in the buffer
        int space_available = BUFFER_SIZE - shm->write_index + shm->read_index;

        // Determine the number of letters that can be written based on the available space
        int num_to_write = space_available >= 20 ? 20 : space_available;

        // Loop through the letters to be written
        for (int i = 0; i < num_to_write; i++) {
            // Write the current letter to the buffer
            shm->buffer[shm->write_index] = letters[i];
    
            // Move the write index to the next available position in the buffer
            shm->write_index = (shm->write_index + 1) % BUFFER_SIZE;
        }
        sem_post(sem);

        // Sleep for 2 seconds
        sleep(2);
    }

    return 0;
}
