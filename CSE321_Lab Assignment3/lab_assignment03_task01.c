#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shared {
    char sel[100];
    int b;
};

int main() {
    int shm_id = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | 0666);
    struct shared *sh = (struct shared *)shmat(shm_id, NULL, 0);

    int fd[2];
    pipe(fd);

    printf("Provide Your Input From Given Options:\n1. Type a to Add Money\n2. Type w to Withdraw Money\n3. Type c to Check Balance\n");
    scanf(" %s", (*sh).sel);
    (*sh).b = 1000;

    printf("Your selection: %s\n", (*sh).sel);

    pid_t pid = fork();
    if (pid == 0) {
        if (strcmp((*sh).sel, "a") == 0) {
            int add_amount;
            printf("Enter amount to be added:\n");
            scanf("%d", &add_amount);
            if (add_amount > 0) {
                (*sh).b += add_amount;
                printf("Balance added successfully\nUpdated balance after addition: %d\n", (*sh).b);
            } else printf("Adding failed, Invalid amount\n");
        } else if (strcmp((*sh).sel, "w") == 0) {
            int withdraw_amount;
            printf("Enter amount to be withdrawn:\n");
            scanf("%d", &withdraw_amount);
            if (withdraw_amount > 0 && withdraw_amount < (*sh).b) {
                (*sh).b -= withdraw_amount;
                printf("Balance withdrawn successfully\nUpdated balance after withdrawal: %d\n", (*sh).b);
            } else printf("Withdrawal failed, Invalid amount\n");
        } else if (strcmp((*sh).sel, "c") == 0) {
            printf("Your current balance is: %d\n", (*sh).b);
        } else printf("Invalid selection\n");

        char msg[] = "Thank you for using\n";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
        shmdt(sh);
        exit(0);
    } else {
        wait(NULL);
        char buf[100];
        close(fd[1]);
        read(fd[0], buf, sizeof(buf));
        printf("%s", buf);
        close(fd[0]);
        shmdt(sh);
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}
