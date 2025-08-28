#include <stdio.h>
#include <stdlib.h>

int main(int argCount, char *argValues[]) {
    int num_len = argCount - 1;
    int nums[num_len], odd[num_len], even[num_len];
    int odd_idx = 0, even_idx = 0;
    
    for (int i = 0; i < num_len; i++) {
        char *pointer;
        nums[i] = (int)strtol(argValues[i + 1], &pointer, 10);

        if (nums[i] % 2 == 0) {
            even[even_idx++] = nums[i];
        } else {
            odd[odd_idx++] = nums[i];
        }
    }

    
    printf("Odd numbers: ");
    
    for (int j = 0; j < odd_idx; j++) {
        printf("%d ", odd[j]);
    }
    
    printf("\n");
    
    printf("Even numbers: ");
    
    for (int k = 0; k < even_idx; k++) {
        printf("%d ", even[k]);
    }
    
    printf("\n");
}
