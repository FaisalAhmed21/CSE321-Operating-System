#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

struct msg {
    long int type;
    char txt[10];
};

int main() {
    key_t key = 5678;
    int msgid;
    struct msg message;
    char workspace[10];

    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Please enter the workspace name:\n");
    scanf("%s", workspace);

    if (strcmp(workspace, "cse321") != 0) {
        printf("Invalid workspace name\n");
        return 0;
    }

    strcpy(message.txt, workspace);
    message.type = 1;
    msgsnd(msgid, &message, sizeof(message.txt), 0);
    printf("Workspace name sent to OTP generator from log in: %s\n\n", message.txt);

    pid_t otp_pid = fork();
    if (otp_pid == 0) {
        msgrcv(msgid, &message, sizeof(message.txt), 1, 0);
        printf("OTP generator received workspace name from log in: %s\n\n", message.txt);

        int otp = getpid();
        sprintf(message.txt, "%d", otp);

        message.type = 2;
        msgsnd(msgid, &message, sizeof(message.txt), 0);
        printf("OTP sent to log in from OTP generator: %s\n", message.txt);

        message.type = 3;
        msgsnd(msgid, &message, sizeof(message.txt), 0);
        printf("OTP sent to mail from OTP generator: %s\n", message.txt);

        pid_t mail_pid = fork();
        if (mail_pid == 0) {
            msgrcv(msgid, &message, sizeof(message.txt), 3, 0);
            printf("Mail received OTP from OTP generator: %s\n", message.txt);

            message.type = 4;
            msgsnd(msgid, &message, sizeof(message.txt), 0);
            printf("OTP sent to log in from mail: %s\n", message.txt);
            exit(0);
        } else {
            wait(NULL);
            exit(0);
        }
    } else {
        wait(NULL);

        char otp1[10], otp2[10];

        msgrcv(msgid, &message, sizeof(message.txt), 2, 0);
        strcpy(otp1, message.txt);

        msgrcv(msgid, &message, sizeof(message.txt), 4, 0);
        strcpy(otp2, message.txt);

        printf("Log in received OTP from OTP generator: %s\n", otp1);
        printf("Log in received OTP from mail: %s\n", otp2);

        if (strcmp(otp1, otp2) == 0) {
            printf("OTP Verified\n");
        } else {
            printf("OTP Incorrect\n");
        }

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}
