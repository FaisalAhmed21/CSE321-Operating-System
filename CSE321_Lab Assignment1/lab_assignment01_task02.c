#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        pid_t grandchild = fork();
        if (grandchild == 0) {
            printf("I am grandchild\n");
        } else {
            wait(NULL);
            printf("I am child\n");
        }
    } else {
        wait(NULL);
        printf("I am parent\n");
    }

    return 0;
}
