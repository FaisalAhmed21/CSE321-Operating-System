#include <stdio.h>
#include <stdlib.h>

int main(int argCount, char *argValues[]) {
    int num_len = argCount - 1;
    int nums[num_len];
    
    for (int i = 0; i < num_len; i++) {
        char *pointer;
        nums[i] = (int) strtol(argValues[i + 1], &pointer, 10);

        if (*pointer != '\0') {
            printf("Invalid number: %s\n", argValues[i + 1]);
            return 1;
        }
    }
    
    for (int j = 0; j < num_len - 1; j++) {
        for (int k = j + 1; k < num_len; k++) {
            if (nums[j] < nums[k]) {
                int temp = nums[j];
                nums[j] = nums[k];
                nums[k] = temp;
            }
        }
    }
    
    printf("Sorted array: ");
    
    for (int x = 0; x < num_len; x++) {
        printf("%d ", nums[x]);
    }
    
    printf("\n");
}
