#include <iostream>
#include <pthread.h>
#include <stack>
using namespace std;

class for_parameters {
    public:
        int start, end, step;
        void (*function)(int);
};

bool still_allocating_work;
const int OMP_NUM_THREADS = 4; // variavel global relativo ao numero de threads
pthread_mutex_t request = PTHREAD_MUTEX_INITIALIZER; // mutex para o schedule dynamic/guided 
pthread_mutex_t write_on_screen = PTHREAD_MUTEX_INITIALIZER; // mutex para escrita
pthread_mutex_t acess_to_task[OMP_NUM_THREADS] = PTHREAD_MUTEX_INITIALIZER; // mutex para acesso ao vetor work
stack<for_parameters> work[OMP_NUM_THREADS]; // vetor de stacks contendo as iteracoes a serem realizadas por cada thread

void funcao(int i) {
    // funcao exemplo
    cout << "Iteracao " << i+1;
}

int max(int a, int b) {
    if(a>b) return a;
    else return b;
}

void* for_thread(void * var) {
    int thread_num = *((int *)var), i;
    for_parameters parameter;
    while(still_allocating_work || !work[thread_num].empty()) {
        if(!work[thread_num].empty()) {
            pthread_mutex_lock(&acess_to_task[thread_num]);
            parameter = work[thread_num].top();
            work[thread_num].pop();
            pthread_mutex_unlock(&acess_to_task[thread_num]);
            for(i = parameter.start; i<=parameter.end; i += parameter.step) {
                pthread_mutex_lock(&write_on_screen);
                parameter.function(i);
                cout << " na thread " << thread_num << endl;
                pthread_mutex_unlock(&write_on_screen);
            }
        }
    }
    free(var);
    pthread_exit(NULL);
}

void omp_for( int inicio , int passo , int final , int schedule , int chunk_size , void (*f)(int) ) {
    int iteration = 0, thread_num = 0, error, temp;
    still_allocating_work = true;
    pthread_t thread[OMP_NUM_THREADS];
    // criando as threads que ficarao em espera
    for(int i = 0; i<OMP_NUM_THREADS; i++) {
        int* num = (int *) malloc(sizeof(int));
        *num = i;
        error = pthread_create(&thread[i], NULL, for_thread, (void*)num);
        if(error != 0) {
            cout << "Erro na criacao de thread";
            exit(1);
        }
    }
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
                pthread_mutex_lock(&acess_to_task[thread_num]);
                work[thread_num].push(parameters);
                pthread_mutex_unlock(&acess_to_task[thread_num]);
                thread_num = (thread_num+1)%OMP_NUM_THREADS;
            }
        }
        case 1: {

        }
        case 2: {

        }
    }
    still_allocating_work = false;
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
    int inicio = 0, passo = 1, final = 100, chunk_size = 10, schedule = 0; // schedule 0 = static, schedule 1 = dynamic, schedule 2 = guided
    omp_for(inicio, passo, final, schedule, chunk_size, funcao);
    return 0;
}
