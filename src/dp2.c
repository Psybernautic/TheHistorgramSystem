#include "../inc/shared_mem.h"

sem_t *sem;
shared_mem_t *shm;

void handle_sigint(int sig) {
    detach_shared_mem(shm);
    sem_close(sem);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [shm_id]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int shm_id = atoi(argv[1]);
    attach_shared_mem(&shm, shm_id);

    // Fork DC process
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Failed to fork DC process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process (DC)
        execl("./dc", "dc", argv[1], argv[2], argv[3], NULL);
        fprintf(stderr, "Failed to launch DC process: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Register signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    // Main loop
    while (1) {
        // Generate one random letter
        char letter;
        srand(time(NULL));
        letter = 'A' + (rand() % 20);

        // Write letter to shared memory
        sem_wait(sem);
        shm->buffer[shm->write_index] = letter;
        shm->write_index = (shm->write_index + 1) % BUFFER_SIZE;
        sem_post(sem);

        // Sleep for 1/20th of a second
        usleep(50000);
    }

    return 0;
}
