#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argCount, char *argValues[]) {
    pid_t pid = fork();

    if (pid == 0) {
        execl("./sort", "sort", argValues[1], argValues[2], argValues[3], argValues[4], argValues[5], NULL);
        perror("execl failed (sort)");
        return 1;
    } else if (pid > 0) {
        wait(NULL);
        execl("./oddeven", "oddeven", argValues[1], argValues[2], argValues[3], argValues[4], argValues[5], NULL);
        perror("execl failed (oddeven)");
        return 1;
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
