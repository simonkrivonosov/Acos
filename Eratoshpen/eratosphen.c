#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"
#include "limits.h"

int* resheto;
int num_of_threads;
void* process(void* task) {
    int* task_data = (int*)task;
    for(int i = task_data[0]; i <= task_data[1] ; ++i) {
        if(i == 0 || i == 1)
            continue;
        if(i % task_data[2] == 0 && i != task_data[2])
            resheto[i] = 1;
    }
    free(task);
    return (void*)0;
}

int main(int argc, char** argv) {

    if(argc < 2){
        perror("invalid number of arguments\n");
        exit(-1);
    }
    int N;
    int sqrtN;
    num_of_threads = atoi(argv[1]);
    pthread_t threads[num_of_threads];
    if(argc == 3) {
        N = atoi(argv[2]);
        sqrtN = (int)sqrt(N);
        resheto = (int*)malloc((N + 1)* sizeof(int));
        if(resheto == NULL) {
            perror("Failed to allocate memory for resheto");
            exit(-1);
        }
        memset(resheto, 0, (N + 1) * sizeof(int));
        int chunk = N / num_of_threads;
        for(int n = 2; n <= sqrtN + 1; ++n) {
            if(resheto[n] == 0) {
                for(int i = 0; i < num_of_threads; ++i) {
                    int* task = (int*)malloc(3 * sizeof(int));
                    task[0] = chunk * i;
                    task[1] = chunk * (i + 1);
                    task[2] = n;
                    if(i + 1 == num_of_threads)
                        task[1] = N;
                    if(n > 2)
                        pthread_join(threads[i], NULL);
                    int status = pthread_create(&threads[i], NULL, process, (void*)task);
                    if(status) {
                        perror("Unable to create thread");
                        exit(-1);
                    }
                }
            }
        }
        for(int i = 0; i < num_of_threads; ++i)
            pthread_join(threads[i], NULL);
        for(int c = 2; c <= N; ++c)
            if(resheto[c] == 0)
                printf("%d ", c);
    }
    if(argc == 2)
    {
        sqrtN = 1;
        while(1){
            sqrtN *= 10;
            N = sqrtN * sqrtN;
            resheto = (int*)malloc((N)* sizeof(int));
            memset(resheto, 0, (N) * sizeof(int));
            if(resheto == NULL) {
                perror("Failed to allocate memory for resheto");
                exit(-1);
            }
            int chunk = N / num_of_threads;
            for(int n = 2; n <= sqrtN + 1; ++n) {
                if(resheto[n] == 0) {
                    for(int i = 0; i < num_of_threads; ++i) {
                        int* task = (int*)malloc(3 * sizeof(int));
                        task[0] = chunk * i;
                        if(n == 2)
                            task[0] += N/100;
                        task[1] = chunk * (i + 1);
                        task[2] = n;
                        if(i + 1 == num_of_threads)
                            task[1] = N;
                        if(n > 2)
                            pthread_join(threads[i], NULL);
                        int status = pthread_create(&threads[i], NULL, process, (void*)task);
                        if(status) {
                            perror("Unable to create thread");
                            exit(-1);
                        }
                    }
                }
            }
            for(int i = 0; i < num_of_threads; ++i)
                pthread_join(threads[i], NULL);
            for(int c = N/100; c <= N; ++c)
                if(resheto[c] != 1 && c != 1)
                    printf("%d ", c);
            free(resheto);
            getchar(); // to get new part of prime numbers
        }
    }

    printf("\n");
    free(resheto);

    return 0;
}

