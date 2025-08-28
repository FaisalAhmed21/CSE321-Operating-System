#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int total_students = 10;
int total_chairs = 3;

int waiting_students = 0;
int students_served = 0;

sem_t student_sem;
sem_t tutor_sem;
pthread_mutex_t lock;

void *student_function(void *arg) {
    int student_id = *((int *)arg);
    sleep(rand() % 3);

    pthread_mutex_lock(&lock);
    if (waiting_students < total_chairs) {
        printf("Student %d started waiting for consultation\n", student_id);
        waiting_students++;
        sem_post(&student_sem);
        pthread_mutex_unlock(&lock);

        sem_wait(&tutor_sem);  
        printf("Student %d is getting consultation\n", student_id);
        sleep(1);  // Simulate consultation time
        printf("Student %d finished getting consultation and left\n", student_id);
    } else {
        printf("No chairs remaining in lobby. Student %d Leaving.....\n", student_id);
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

void *tutor_function(void *arg) {
    while (students_served < total_students) {
        sem_wait(&student_sem); 

        pthread_mutex_lock(&lock);
        printf("A waiting student started getting consultation\n");
        waiting_students--;
        printf("Number of students now waiting: %d\n", waiting_students);
        printf("ST giving consultation\n");
        pthread_mutex_unlock(&lock);

        sem_post(&tutor_sem); 
        sleep(1); 

        pthread_mutex_lock(&lock);
        students_served++;
        printf("Number of served students: %d\n", students_served);
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t student_threads[10];
    pthread_t tutor_thread;
    int student_ids[10];

    int i;
    sem_init(&student_sem, 0, 0);
    sem_init(&tutor_sem, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&tutor_thread, NULL, tutor_function, NULL);

    for (i = 0; i < 10; i++) {
        student_ids[i] = i;  // assign student ID
        pthread_create(&student_threads[i], NULL, student_function, &student_ids[i]);
    }

    for (i = 0; i < 10; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_join(tutor_thread, NULL);
    sem_destroy(&student_sem);
    sem_destroy(&tutor_sem);
    pthread_mutex_destroy(&lock);

    return 0;
}

