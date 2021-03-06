/*THIS IS THE CONSUMER PROCESS*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

struct studentInfo
{
    char name[100];
    char roll[100];
    int age;
    char section[100];
};

typedef struct
{
    int in;
    int out;
    int prid;
    int cnid;
    struct studentInfo buffer[10];
} mydata;

void my_handler() {}

int main()
{
    printf("Consumer Program Started...\n");
    signal(SIGUSR1, my_handler);
    int status;
    int shmid;
    int i;
    mydata *data;
    key_t key;
    key = ftok("empty.txt", 11);
    if (key < 0)
    {
        perror("error0:");
        exit(1);
    }
    shmid = shmget(key, sizeof(mydata), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("error1:");
        exit(1);
    }
    data = (mydata *)shmat(shmid, NULL, 0);
    if (data == (void *)-1)
    {
        perror("error2:");
        exit(1);
    }

    data->cnid = getpid();
    data->out = 0;
    do
    {
        pause();
        i = data->out;
        printf("Enter Student Information :\n");
        printf("Name : %s\n", (data->buffer[i]).name);
        printf("Roll Number : %s\n", (data->buffer[i]).roll);
        printf("Age : %d\n", (data->buffer[i]).age);
        printf("Section : %s\n", (data->buffer[i]).section);
        data->out = data->out + 1;
        kill(data->prid, SIGUSR1);
    } while (1);

    printf("Consumer Program Exting...\n");
}