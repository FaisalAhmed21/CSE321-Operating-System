#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main() {
    int *total_process = mmap(NULL, sizeof *total_process,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *total_process = 1; 
    pid_t parent = getpid();

    if (fork() == 0) {
        printf("The pid is %d\n", getpid());
        if (getpid() % 2 != 0) {
            fork(); 
            (*total_process)++;
        }
        return 0;
    } else {
        (*total_process)++;
    }

    if (fork() == 0) {
        printf("The pid is %d\n", getpid());
        if (getpid() % 2 != 0) {
            fork();
            (*total_process)++;
        }
        return 0;
    } else {
        (*total_process)++;
    }

    if (fork() == 0) {
        printf("The pid is %d\n", getpid());
        if (getpid() % 2 != 0) {
            fork();
            (*total_process)++;
        }
        return 0;
    } else {
        (*total_process)++;
    }

    wait(NULL);
    if (getpid() == parent) {
        printf("Total number of processes that have been created: %d\n", *total_process);
    }

    return 0;
}
