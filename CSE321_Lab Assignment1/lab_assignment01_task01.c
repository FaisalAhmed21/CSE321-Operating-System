#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argCount, char *argValues[]) {
    char string[1024];
    char *file_name;
    if (argCount == 1){
        file_name = "output.txt";
    }
    else if (argCount == 2){
        file_name = argValues[1];
    }
    else {
        printf("Usage: %s <filename>\n", argValues[0]);
        return 1;
    }

    int fd = open(file_name, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("File opening failed");
        return 1;
    }

    while (1) {
        printf("Enter a string ((-1) to stop): ");
        fgets(string, 1024, stdin);
        if (strcmp(string, "-1\n") == 0) {
            break;
        }
        write(fd, string, strlen(string));
    }
    
    close(fd);
    printf("File closed successfully\n");
    return 0;
}
