#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int max_wait_time;
typedef struct {
    int *buffer;
    int size;
    int front;
    int rear;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} ColaCircular;

// Inicia la cola (le asigna el espacio de memoria)
void iniciarCola(ColaCircular *q, int size) {
    q->size = size;
    q->buffer = (int *)malloc(size * sizeof(int));
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

// Revisa si la cola esta llena
bool isFull(ColaCircular *q) {
    return q->count == q->size;
}

// Revisa si la cola esta vacia
bool isEmpty(ColaCircular *q) {
    return q->count == 0;
}

// Le cambia el size a la cola segun lo que se requiera
void resizeQueue(ColaCircular *q) {
    if (isFull(q)) {
        q->size *= 2;
        std::cout << "Size de la cola reasignado: nuevo size: " << q->size << std::endl;
    } else if (q->count <= q->size / 4) {
        q->size /= 2;
        std::cout << "Size de la cola reasignado: nuevo size: " << q->size << std::endl;
    }
    q->buffer = (int *)realloc(q->buffer, q->size * sizeof(int));
}

// Agrega un item a la cola
void enqueue(ColaCircular *q, int item) {
    q->buffer[q->rear] = item;
    q->rear = (q->rear + 1) % q->size;
    q->count++;
    return;
}

// Saca un item de la cola
int dequeue(ColaCircular *q) {
    int item = q->buffer[q->front];
    q->front = (q->front + 1) % q->size;
    q->count--;
    return item;
}

// Produce items para la cola
int item = 0;
void *productor(void *arg) {
    ColaCircular *q = (ColaCircular *)arg;
    return NULL;
}

// Consume items de la cola
void *consumidor(void *arg) {
    ColaCircular *q = (ColaCircular *)arg;
    return NULL;
}


int main(int argc, char *argv[]) {
    if (argc < 9) {
        std::cerr << "Uso incorrecto: " << argv[0] << " -p <numero-productores> -c <numero-consumidores> -s <size-inicial-cola> -t <tiempo-espera-max-consumidor>" << std::endl;
        return 1;
    }

    int num_productores;
    int num_consumidores;
    int initial_size;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            num_productores = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            num_consumidores = atoi(argv[++i]); 
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            initial_size = atoi(argv[++i]); 
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc){
            max_wait_time = atoi(argv[++i]);
        } else {
            std::cerr << "Argumento desconocido: " << argv[i] << std::endl;
            return 1;
        }
    }

    ColaCircular queue;
    iniciarCola(&queue, initial_size);

    pthread_t productores[num_productores];
    pthread_t consumidores[num_consumidores];

    for (int i = 0; i < num_productores; i++) {
        pthread_create(&productores[i], NULL, productor, (void *)&queue);
    }

    for (int i = 0; i < num_consumidores; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, (void *)&queue);
    }

    for (int i = 0; i < num_productores; i++) {
        pthread_join(productores[i], NULL);
    }

    for (int i = 0; i < num_consumidores; i++) {
        pthread_join(consumidores[i], NULL);
    }

    // Limpiar
    free(queue.buffer);
    pthread_mutex_destroy(&queue.lock);
    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.not_empty);

    return 0;
}
