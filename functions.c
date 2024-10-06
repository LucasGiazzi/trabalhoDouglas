#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define SIZE 30

extern int matriz[SIZE][SIZE];
extern pthread_mutex_t mutex_matriz;
extern pthread_cond_t cond_var;

void inicializar_matriz() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matriz[i][j] = 0;
        }
    }
}

void exibir_matriz() {
    pthread_mutex_lock(&mutex_matriz);
    printf("\nEstado atual da matriz:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matriz[i][j] == 0)
                printf("- ");
            else if (matriz[i][j] == 1)
                printf("T ");
            else if (matriz[i][j] == 2)
                printf("@ ");
            else
                printf("/ ");
        }
        printf("\n");
    }
    pthread_mutex_unlock(&mutex_matriz);
}

void* sensor(void* arg) {
    int index = (intptr_t)arg;
    int x = index / SIZE;
    int y = index % SIZE;

    while (1) {
        pthread_mutex_lock(&mutex_matriz);
        if (matriz[x][y] == 2) {
            printf("Sensor em (%d, %d) detectou fogo! Enviando alerta...\n", x, y);
            matriz[x][y] = 3;
            pthread_cond_broadcast(&cond_var);
        }
        pthread_mutex_unlock(&mutex_matriz);
        sleep(1);
    }
    return NULL;
}

void* gerador_de_incendios(void* arg) {
    while (1) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        pthread_mutex_lock(&mutex_matriz);
        matriz[x][y] = 2;
        printf("Incêndio gerado na posição (%d, %d)\n", x, y);
        pthread_mutex_unlock(&mutex_matriz);

        exibir_matriz();
        sleep(3);
    }
    return NULL;
}

void* central_de_controle(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex_matriz);
        pthread_cond_wait(&cond_var, &mutex_matriz);
        pthread_mutex_unlock(&mutex_matriz);
        sleep(5);
    }
    return NULL;
}
