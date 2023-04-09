#include "../../common/inc/shared_mem.h"



int letter_counts[NUM_LETTERS] = {0};
int sharedMemoryID, semaphoreID, dp1_pid, dp2_pid;

void update_histogram() {
    system("clear");

    for (int i = 0; i < NUM_LETTERS; ++i) {
        char letter = 'A' + i;
        int count = letter_counts[i];

        printf("%c-%03d ", letter, count);

        int hundreds = count / 100;
        int tens = (count % 100) / 10;
        int ones = count % 10;

        for (int j = 0; j < hundreds; ++j) printf("*");
        for (int j = 0; j < tens; ++j) printf("+");
        for (int j = 0; j < ones; ++j) printf("-");

        printf("\n");
    }
}

void handle_sigint(int sig) {
    kill(dp1_pid, SIGINT);
    kill(dp2_pid, SIGINT);

    shared_mem_t *shm_ptr = (shared_mem_t *)shmat(sharedMemoryID, NULL, 0);

    while (shm_ptr->read_index != shm_ptr->write_index) {
        usleep(1000);
    }

    update_histogram();

    shmdt(shm_ptr);
    shmctl(sharedMemoryID, IPC_RMID, NULL);
    semctl(semaphoreID, 0, IPC_RMID);

    printf("\nShazam !!\n");

    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <sharedMemoryID> <DP1_PID> <DP2_PID>\n", argv[0]);
        return 1;
    }

    sharedMemoryID = atoi(argv[1]);
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[3]);

    shared_mem_t *shm_ptr = (shared_mem_t *)shmat(sharedMemoryID, NULL, 0);
    semaphoreID = semget(SHM_KEY, 1, 0666);

    signal(SIGINT, handle_sigint);

    while (1) {
        usleep(2000000);

        struct sembuf sem_op = {0, -1, 0};
        semop(semaphoreID, &sem_op, 1);

        while (shm_ptr->read_index != shm_ptr->write_index) {
            char letter = shm_ptr->buffer[shm_ptr->read_index];
            letter_counts[letter - 'A']++;

            shm_ptr->read_index = (shm_ptr->read_index + 1) % BUFFER_SIZE;
        }

        sem_op.sem_op = 1;
        semop(semaphoreID, &sem_op, 1);

        static int counter = 0;
        counter++;

        if (counter >= 5) {
            counter = 0;
            update_histogram();
        }
    }

    return 0;
}
