#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// definindo tempo de execucao em segundos
#define EXECUTION_TIME 60

// definindo a quantidade de recursos
#define NUM_PC 10
#define NUM_VR 6
#define NUM_CHAIR 8

// armazenamento de estatisticas
int customers_total = 0;
double waiting_time = 0;
int customers_working = 0;
int custumers_without_resourses = 0;
int pcs_used = 0;
int vrs_used = 0;
int chairs_used = 0;

// definicao dos tipos de clientes
typedef enum {GAMER, FREELANCE, STUDENT} Type_Customer;

// criando semaforos e mutex
sem_t pc_sem;
sem_t vr_sem;
sem_t chair_sem;

pthread_mutex_t statsLock;

// imprimindo status em tempo real
void print_status() {
    system("clear");
    int value_pc, value_vr, value_chair;

    sem_getvalue(&pc_sem, &value_pc);
    sem_getvalue(&vr_sem, &value_vr);
    sem_getvalue(&chair_sem, &value_chair);

    printf("PCs: %d\tVRs: %d\tCadeiras: %d\n", value_pc, value_vr, value_chair);
    printf("Clientes na loja: %d\n", customers_working);
    printf("Clientes atendidos: %d\n", customers_total);
    printf("Clientes nao atendidos: %d\n", custumers_without_resourses);
}

// atualizando estatisticas gerais
void update_stats(int working_change, int served_change, int without_resourses_change) {
    pthread_mutex_lock(&statsLock);
    customers_working += working_change;
    customers_total += served_change;
    custumers_without_resourses += without_resourses_change;
    pthread_mutex_unlock(&statsLock);
}

// atualizando estatisticas de uso de recursos
void update_uses(int pc_change, int vr_change, int chair_change) {
    pthread_mutex_lock(&statsLock);
    pcs_used += pc_change;
    vrs_used += vr_change;
    chairs_used += chair_change;
    pthread_mutex_unlock(&statsLock);
}

// atualizando tempo de espera total
void update_time(double time) {
    pthread_mutex_lock(&statsLock);
    waiting_time += time;
    pthread_mutex_unlock(&statsLock);
}

// funcao a ser executada pela thread
void* use_cyber_cafe(void* arg) {
    Type_Customer customer = rand() % 3;

    time_t begin, end;
    double time_awaited;

    print_status();
    
    if (customer == GAMER) { // caso seja gamer tenta pegar PC e VR
        begin = time(NULL);
        int got_resources = 0;
        while (difftime(time(NULL), begin) < 3 + (rand() % 10)) {
            if (sem_trywait(&pc_sem) == 0) {
                if (sem_trywait(&vr_sem) == 0) {
                    got_resources = 1;
                    break;
                } 
            } else {
                sem_post(&pc_sem);
            }
            usleep(200000);
        }

        
        if (got_resources == 1) { // caso consiga, tenta a cadeira tambem, mas sem espera
            int get_chair = 0;
            if (sem_trywait(&chair_sem) == 0) {
                get_chair = 1;
            }

            update_stats(1, 0, 0);

            update_time(difftime(time(NULL), begin));

            // simulando o uso dos recursos
            usleep(5000000 + rand() % 20000000);

            sem_post(&pc_sem);
            sem_post(&vr_sem);
            if (get_chair == 1) {
                sem_post(&chair_sem);
            }

            update_stats(-1, 1, 0);
            update_uses(1, 1, get_chair);
        } else {
            update_time(difftime(time(NULL), begin));
            update_stats(0, 0, 1);
        }
    } else if (customer == FREELANCE) { // caso seja gamer tenta pegar PC e cadeira
        begin = time(NULL);

        int got_resources = 0;
        while (difftime(time(NULL), begin) < 3 + (rand() % 10)) {
            if (sem_trywait(&pc_sem) == 0) {
                if (sem_trywait(&chair_sem) == 0) {
                    got_resources = 1;
                    break;
                } 
            } else {
                sem_post(&pc_sem);
            }
            usleep(200000);
        }

        if (got_resources == 1) { // caso consiga, tenta o VR tambem, mas sem espera
            int get_vr = 0;
            if (sem_trywait(&vr_sem) == 0) {
                get_vr = 1;
            }

            update_stats(1, 0, 0);

            update_time(difftime(time(NULL), begin));

            usleep(5000000 + rand() % 20000000);

            sem_post(&pc_sem);
            sem_post(&chair_sem);
            if (get_vr == 1) {
                sem_post(&vr_sem);
            }

            update_stats(-1, 1, 0);
            update_uses(1, 1, get_vr);
        } else {
            update_time(difftime(time(NULL), begin));
            update_stats(0, 0, 1);
        }
    } else if (customer == STUDENT) { // caso seja estudante, pegue apenas o PC
        begin = time(NULL);

        int got_resources = 0;
        while (difftime(time(NULL), begin) < 3 + (rand() % 10)) {
            if (sem_trywait(&pc_sem) == 0) {
                got_resources = 1; 
                break;
            }
            usleep(200000);
        }

        if (got_resources == 1) {
            update_stats(1, 0, 0);
            
            update_time(difftime(time(NULL), begin));

            usleep(5000000 + rand() % 20000000);
    
            sem_post(&pc_sem);
    
            update_stats(-1, 1, 0);
            update_uses(1, 0, 0);
        } else {
            update_time(difftime(time(NULL), begin));
            update_stats(0, 0, 1);
        } 
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    time_t start_time = time(NULL);

    // quantidade arbitraria de threads
    pthread_t threads[1000];

    // iniciamos os semaforos
    sem_init(&pc_sem, 0, NUM_PC);
    sem_init(&vr_sem, 0, NUM_VR);
    sem_init(&chair_sem, 0, NUM_CHAIR);

    // criacao das threads 
    long currentThread = 0;
    while(time(NULL) - start_time < EXECUTION_TIME) {
        pthread_create(&threads[currentThread], NULL, use_cyber_cafe, (void *)currentThread);
        
        usleep(rand() % 3000000);
        currentThread++;
    }

    // unimos as threads para finalizar
    for (int i = 0; i <= currentThread; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n--Fechou--\n");
    printf("Clientes atendidos: %d\n", customers_total);
    printf("Tempo medio de espera: %f\n", waiting_time / (double)customers_total);
    printf("Clientes nao atendidos: %d\n", custumers_without_resourses);
    printf("PCs usados: %d\n", pcs_used);
    printf("VRs usados: %d\n", vrs_used);
    printf("Cadeiras usadas: %d\n", chairs_used);

    sem_destroy(&pc_sem);
    sem_destroy(&vr_sem);
    sem_destroy(&chair_sem);
}