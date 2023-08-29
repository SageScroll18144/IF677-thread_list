#include <iostream>
#include <pthread.h>
#include <stack>
#include <cmath>
using namespace std;

class for_parameters {
    public:
        int start, end, step;
        void (*function)(int);
};
const int OMP_NUM_THREADS = 4; // variavel global relativo ao numero de threads
pthread_mutex_t write_on_screen = PTHREAD_MUTEX_INITIALIZER; // mutex para escrita
bool task[OMP_NUM_THREADS]; // semaforo para as threads
bool not_finished, wait; // variaveis auxiliares
stack<for_parameters> work[OMP_NUM_THREADS]; // vetor de stacks contendo as iteracoes a serem realizadas por cada thread

void funcao(int i) {
    // funcao exemplo
    cout << "Iteracao " << i+1;
}

void* for_thread_static(void * var) {
    int thread_num = *((int *)var), i;
    for_parameters parameter;
    while(!work[thread_num].empty()) {
        parameter = work[thread_num].top();
        work[thread_num].pop();
        for(i = parameter.start; i<=parameter.end; i += parameter.step) {
            pthread_mutex_lock(&write_on_screen);
            parameter.function(i);
            cout << " na thread " << thread_num << endl;
            pthread_mutex_unlock(&write_on_screen);
        }
    }
    free(var);
    pthread_exit(NULL);
}

void* for_thread_dynamic(void * var) {
    int thread_num = *((int *)var), i;
    for_parameters parameter;
    while(not_finished) {
        if(task[thread_num]) {
            parameter = work[thread_num].top();
            work[thread_num].pop();
            for(i = parameter.start; i<=parameter.end; i += parameter.step) {
                pthread_mutex_lock(&write_on_screen);
                parameter.function(i);
                cout << " na thread " << thread_num << endl;
                pthread_mutex_unlock(&write_on_screen);
            }
            task[thread_num] = false;
        }
    }
    free(var);
    pthread_exit(NULL);
}

void omp_for( int inicio , int passo , int final , int schedule , int chunk_size , void (*f)(int) ) {
    int iteration = inicio, thread_num = 0, error, temp;
    pthread_t thread[OMP_NUM_THREADS];
    //inserindo as iteracoes a serem feitas por cada thread de acordo com o schedule
    switch(schedule) {
        case 0: {
            int tasknum = chunk_size;
            while(iteration < final) {
                if(thread_num == 0 && final-iteration<OMP_NUM_THREADS*tasknum) tasknum = max((final-iteration)/OMP_NUM_THREADS,1);
                if(iteration+(tasknum*passo)-1 >= final) temp = final-1;
                else temp = iteration+(tasknum*passo)-1;
                for_parameters parameters = {iteration, temp, passo, f};
                iteration += tasknum*passo;
                work[thread_num].push(parameters);
                thread_num = (thread_num+1)%OMP_NUM_THREADS;
            }
            for(int i = 0; i<OMP_NUM_THREADS; i++) {
                int* num = (int *) malloc(sizeof(int));
                *num = i;
                error = pthread_create(&thread[i], NULL, for_thread_static, (void*)num);
                if(error != 0) {
                    cout << "Erro na criacao de thread";
                    exit(1);
                }
            }
        } break;
        case 1: {
            int iteration = inicio, thread_num = 0, i;
            not_finished = true; wait = false;
            bool check;
            for(int i = 0; i<OMP_NUM_THREADS; i++) {
                int* num = (int *) malloc(sizeof(int));
                *num = i;
                error = pthread_create(&thread[i], NULL, for_thread_dynamic, (void*)num);
                if(error != 0) {
                    cout << "Erro na criacao de thread";
                    exit(1);
                }
                access_to_task[i] = PTHREAD_MUTEX_INITIALIZER;
                task[i] = false;
            }
            while(not_finished) {
                if(wait) {
                    check = true;
                    for(i = 0; i<OMP_NUM_THREADS; i++) {
                        check = check && !task[i];
                    }
                    if(check) {
                        not_finished = false;
                    }
                }
                else if(!task[thread_num]) {
                    if(iteration+(chunk_size*passo)-1 >= final) temp = final-1;
                    else temp = iteration+(chunk_size*passo)-1;
                    for_parameters parameters = {iteration, temp, passo, f};
                    iteration += chunk_size*passo;
                    work[thread_num].push(parameters);
                    task[thread_num] = true;
                    if(iteration >= final) wait = true;
                }
                thread_num = (thread_num+1)%OMP_NUM_THREADS;
            }
        } break;
        case 2: {
            int iteration = inicio, thread_num = 0, i, tasknum = chunk_size;
            not_finished = true; wait = false;
            bool check;
            for(int i = 0; i<OMP_NUM_THREADS; i++) {
                int* num = (int *) malloc(sizeof(int));
                *num = i;
                error = pthread_create(&thread[i], NULL, for_thread_dynamic, (void*)num);
                if(error != 0) {
                    cout << "Erro na criacao de thread";
                    exit(1);
                }
                access_to_task[i] = PTHREAD_MUTEX_INITIALIZER;
                task[i] = false;
            }
            while(not_finished) {
                if(wait) {
                    check = true;
                    for(i = 0; i<OMP_NUM_THREADS; i++) {
                        check = check && !task[i];
                    }
                    if(check) {
                        not_finished = false;
                    }
                }
                else if(!task[thread_num]) {
                    tasknum = max<int>(ceil((double)(final-iteration)/(double)OMP_NUM_THREADS), chunk_size);
                    if(iteration+(tasknum*passo)-1 >= final) temp = final-1;
                    else temp = iteration+(tasknum*passo)-1;
                    for_parameters parameters = {iteration, temp, passo, f};
                    iteration += tasknum*passo;
                    work[thread_num].push(parameters);
                    task[thread_num] = true;
                    if(iteration >= final) wait = true;
                }
                thread_num = (thread_num+1)%OMP_NUM_THREADS;
            }
        } break;
    }
    for(int i = 0; i<OMP_NUM_THREADS; i++) { // espera ate todas as iteracoes acabarem
        pthread_join(thread[i], NULL);
    }
    return;
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);
    // queremos simular o seguinte macro
    /*#pragma omp for
    for(int i = inicio ; i < final ; i += passo )
    {   
    f(i);
    }*/
    int inicio = 0, passo = 1, final = 15, chunk_size = 2, schedule = 0; // schedule 0 = static, schedule 1 = dynamic, schedule 2 = guided
    omp_for(inicio, passo, final, schedule, chunk_size, funcao);
    return 0;
}
