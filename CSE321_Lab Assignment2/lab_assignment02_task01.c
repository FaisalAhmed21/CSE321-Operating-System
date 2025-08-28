#include <stdio.h>
#include <pthread.h>

long long fib[41];
int last_term;

struct SearchData {
    int *positions;    
    long long *results; 
    int count;
};

void* generate_fib(void* arg) {
    int n = *(int*)arg;
    fib[0] = 0;
    if (n >= 1) fib[1] = 1;
    for (int i = 2; i <= n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    return NULL;
}

void* search_fib(void* arg) {
    struct SearchData* data = (struct SearchData*)arg;

    for (int i = 0; i < data->count; i++) {
        int pos = data->positions[i];
        if (pos >= 0 && pos <= last_term) {
            data->results[i] = fib[pos];
        } else {
            data->results[i] = -1;
        }
    }
    return NULL;
}

int main() {
    pthread_t thread_gen, thread_search;
    struct SearchData data;
    int search_count;

    while (1) {
        printf("Enter the term of Fibonacci sequence (0–40): ");
        if (scanf("%d", &last_term) != 1) {
            printf("Invalid input! Please enter an integer.\n");
            while (getchar() != '\n'); 
            continue;
        }
        if (last_term >= 0 && last_term <= 40) break;
        printf("Error: term must be between 0 and 40.\n");
    }

    pthread_create(&thread_gen, NULL, generate_fib, &last_term);
    pthread_join(thread_gen, NULL);

    for (int i = 0; i <= last_term; i++) {
        printf("F[%d] = %lld\n", i, fib[i]);
    }

    while (1) {
        printf("How many numbers you are willing to search? (>0): ");
        if (scanf("%d", &search_count) != 1) {
            printf("Invalid input! Please enter an integer.\n");
            while (getchar() != '\n');
            continue;
        }
        if (search_count > 0) break;
        printf("Error: number of searches must be greater than 0.\n");
    }

    int positions[search_count];
    long long results[search_count];

    for (int i = 0; i < search_count; i++) {
        while (1) {
            printf("Enter search %d: ", i + 1);
            if (scanf("%d", &positions[i]) != 1) {
                printf("Invalid input! Please enter an integer.\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        }
    }

    data.positions = positions;
    data.results = results;
    data.count = search_count;

    pthread_create(&thread_search, NULL, search_fib, &data);
    pthread_join(thread_search, NULL);

    for (int i = 0; i < search_count; i++) {
        printf("Result of search #%d = %lld\n", i + 1, results[i]);
    }

    return 0;
}
