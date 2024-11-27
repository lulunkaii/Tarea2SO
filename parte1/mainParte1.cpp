#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int max_wait_time;
FILE *log_file;
int item = 0;

typedef struct {
    int *buffer;
    int size;
    int front;
    int rear;
    int count;
    int productores_activos;
    int tiempo_espera_max_consumidor;
    bool hay_productores;
    bool timer_activo;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    pthread_cond_t productores_terminados;
} ColaCircular;

// Inicia la cola (le asigna el espacio de memoria)
void iniciarCola(ColaCircular *q, int size, int productores_activos, int tiempo_espera_max_consumidor) {
    q->size = size;
    q->buffer = (int *)malloc(size * sizeof(int));
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    q->productores_activos = productores_activos;
    q->tiempo_espera_max_consumidor = tiempo_espera_max_consumidor;
    q->hay_productores = true;
    q->timer_activo = false;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->productores_terminados, NULL);
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
        fprintf(log_file, "Size de la cola reasignado: nuevo size: %d\n", q->size);
    } else if (q->count <= q->size / 4) {
        q->size /= 2;
        fprintf(log_file, "Size de la cola reasignado: nuevo size: %d\n", q->size);
    }
    q->buffer = (int *)realloc(q->buffer, q->size * sizeof(int));
}

// Agrega un item a la cola
void enqueue(ColaCircular *q, int item) {
    pthread_mutex_lock(&q->lock);
    if (isFull(q)) {
        resizeQueue(q);
        pthread_cond_broadcast(&q->not_full);
    }
    q->buffer[q->rear] = item;
    q->rear = (q->rear + 1) % q->size;
    q->count++;
    pthread_mutex_unlock(&q->lock);
    pthread_cond_signal(&q->not_empty);
}

// Saca un item de la cola
int dequeue(ColaCircular *q) {
    pthread_mutex_lock(&q->lock);
    while (isEmpty(q) && (q->hay_productores || q->timer_activo)) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += max_wait_time; // Espera máxima
        if (pthread_cond_timedwait(&q->not_empty, &q->lock, &ts) == ETIMEDOUT) {
            pthread_mutex_unlock(&q->lock);
            return -1; // Indicar que se alcanzó el tiempo máximo de espera
        }
    }
    if (isEmpty(q)) {
        pthread_mutex_unlock(&q->lock);
        return -1;
    }
    int item = q->buffer[q->front];
    q->front = (q->front + 1) % q->size;
    q->count--;
    if (q->count <= q->size / 4) {
        resizeQueue(q); // Reducir tamaño de la cola si su uso llega al 25%
        pthread_cond_broadcast(&q->not_full); // Notificar a todos los productores
    }
    pthread_mutex_unlock(&q->lock);
    pthread_cond_signal(&q->not_full);
    return item;
}

// Produce items para la cola
void *productor(void *arg) {
    ColaCircular *q = (ColaCircular *)arg;
    
    enqueue(q, item);
    fprintf(log_file, "Productor: %d\n", item);
    item++;
    usleep(100000); // Pequeño retraso para simular trabajo
    
    pthread_mutex_lock(&q->lock);
    q->productores_activos--;
    if (q->productores_activos == 0) {
        q->hay_productores = false;
        q->timer_activo = true;
        pthread_cond_broadcast(&q->productores_terminados);
    }
    pthread_mutex_unlock(&q->lock);
    
    return NULL;
}

// Consume items de la cola
void *consumidor(void *arg) {
    ColaCircular *q = (ColaCircular *)arg;
    while (true) {
        int itemConsumido = dequeue(q);
        if (itemConsumido != -1) {
            fprintf(log_file, "Consumidor: %d\n", itemConsumido);
        } else {
            fprintf(log_file, "Consumidor: Tiempo de espera máximo alcanzado\n");
        }
        usleep(150000); // Pequeño retraso para simular trabajo
        
        pthread_mutex_lock(&q->lock);
        if (!q->hay_productores && !q->timer_activo) {
            pthread_mutex_unlock(&q->lock);
            break;
        }
        pthread_mutex_unlock(&q->lock);
    }

    return NULL;
}

void *temporizador(void *arg) {
    ColaCircular *q = (ColaCircular *)arg;
    pthread_mutex_lock(&q->lock);
    while (q->hay_productores) {
        pthread_cond_wait(&q->productores_terminados, &q->lock);
    }
    pthread_mutex_unlock(&q->lock);

    sleep(q->tiempo_espera_max_consumidor);

    pthread_mutex_lock(&q->lock);
    q->timer_activo = false;
    pthread_cond_broadcast(&q->not_empty);
    pthread_mutex_unlock(&q->lock);

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
    
    // Abrir archivo para hacer el log
    log_file = fopen("log.txt", "w");
    if (log_file == NULL) {
        std::cerr << "No se pudo abrir el archivo de log." << std::endl;
        return 1;
    }

    ColaCircular queue;
    iniciarCola(&queue, initial_size, num_productores, max_wait_time);

    pthread_t productores[num_productores];
    pthread_t consumidores[num_consumidores];
    pthread_t timer_thread;

    for (int i = 0; i < num_productores; i++) {
        pthread_create(&productores[i], NULL, productor, (void *)&queue);
        usleep(50000); // Pequeño retraso para permitir que los hilos se inicien
    }

    for (int i = 0; i < num_consumidores; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, (void *)&queue);
        usleep(50000); // Pequeño retraso para permitir que los hilos se inicien
    }

    pthread_create(&timer_thread, NULL, temporizador, (void *)&queue);
    
    for (int i = 0; i < num_productores; i++) {
        pthread_join(productores[i], NULL);
    }

    pthread_join(timer_thread, NULL);

    for (int i = 0; i < num_consumidores; i++) {
        pthread_join(consumidores[i], NULL);
    }

    // Limpiar
    free(queue.buffer);
    pthread_mutex_destroy(&queue.lock);
    pthread_cond_destroy(&queue.not_full);
    pthread_cond_destroy(&queue.not_empty);
    pthread_cond_destroy(&queue.productores_terminados);

    fclose(log_file);

    return 0;
}