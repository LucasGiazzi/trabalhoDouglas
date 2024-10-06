#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define SIZE 30

int matriz[SIZE][SIZE];
pthread_mutex_t mutex_matriz;
pthread_cond_t cond_var;

int main() {
    srand(time(NULL));

    pthread_mutex_init(&mutex_matriz, NULL);
    pthread_cond_init(&cond_var, NULL);

    pthread_t sensores[SIZE * SIZE];
    pthread_t gerador_thread;
    pthread_t central_thread;

    inicializar_matriz();

    pthread_create(&gerador_thread, NULL, gerador_de_incendios, NULL);

    pthread_create(&central_thread, NULL, central_de_controle, NULL);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pthread_create(&sensores[i * SIZE + j], NULL, sensor, (void *)(intptr_t)(i * SIZE + j));
        }
    }

    pthread_join(gerador_thread, NULL);
    pthread_join(central_thread, NULL);

    pthread_mutex_destroy(&mutex_matriz);
    pthread_cond_destroy(&cond_var);

    return 0;
}
