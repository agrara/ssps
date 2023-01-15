#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMS 101

int main()
{

    int fd[2];
    if (pipe(fd) < 0)
    {
        fprintf(stderr, "Creating of a pipe failed");
        return EXIT_FAILURE;
    }

    pid_t p;
    p = fork();

    if (p < 0)
    {
        fprintf(stderr, "Creating of a child process failed");
        return EXIT_FAILURE;
    }
    else if (p == 0)
    {
        close(fd[0]);
        int *numsArray;
        numsArray = (int *)malloc(sizeof(int) * NUMS);
        for (int i = 0; i < NUMS; i++)
        {
            numsArray[i] = i;
        }

        if (write(fd[1], numsArray, sizeof(int) * NUMS) < 0)
        {
            fprintf(stderr, "Can't send data to the parent process");
            return 2;
        }
        close(fd[1]);
        free(numsArray);
    }
    else
    {
        close(fd[1]);
        int numsArrayParent[NUMS];
        if (read(fd[0], numsArrayParent, sizeof(int) * NUMS) < 0)
        {
            fprintf(stderr, "Can't receive data from the child process");
            return 3;
        }
        close(fd[0]);
        wait(NULL);

        int index = 0;
        sem_t semaphore;
        pthread_mutex_t mutex;

        sem_init(&semaphore, 0, 1);
        pthread_mutex_init(&mutex, NULL);

        void *routine1()
        {
            int range = (NUMS % 2 == 0) ? NUMS / 2 : NUMS / 2 + 1;
            for (int i = 0; i < range; i++)
            {
                sem_wait(&semaphore);
                if (numsArrayParent[index] % 15 == 0)
                {
                    printf("ThreeFive");
                }
                else if (numsArrayParent[index] % 5 == 0)
                {
                    printf("Five");
                }
                else if (numsArrayParent[index] % 3 == 0)
                {
                    printf("Three");
                }
                else
                {
                    printf("%d", *(numsArrayParent + index));
                }
                putchar('\n');
                index++;
                sem_post(&semaphore);
            }
        }

        void *routine2()
        {
            for (int i = 0; i < NUMS / 2; i++)
            {
                pthread_mutex_lock(&mutex);
                if (numsArrayParent[index] % 15 == 0)
                {
                    printf("ThreeFive");
                }
                else if (numsArrayParent[index] % 5 == 0)
                {
                    printf("Five");
                }
                else if (numsArrayParent[index] % 3 == 0)
                {
                    printf("Three");
                }
                else
                {
                    printf("%d", *(numsArrayParent + index));
                }
                putchar('\n');
                index++;
                pthread_mutex_unlock(&mutex);
            }
        }

        pthread_t t1, t2;

        if (pthread_create(&t1, NULL, &routine1, NULL) != 0)
        {
            fprintf(stderr, "Unable to create thread t1");
        }

        if (pthread_create(&t2, NULL, &routine2, NULL) != 0)
        {
            fprintf(stderr, "Unable to create thread t2");
        }
        if (pthread_join(t1, NULL) != 0)
        {
            fprintf(stderr, "Unable to join thread t1");
        }
        sem_destroy(&semaphore);
        if (pthread_join(t2, NULL) != 0)
        {
            fprintf(stderr, "Unable to join thread t2");
        }
        pthread_mutex_destroy(&mutex);
    }
    return EXIT_SUCCESS;
}