#include <iostream>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read = PTHREAD_COND_INITIALIZER;
pthread_cond_t write = PTHREAD_COND_INITIALIZER;

int *array;
int readers = 0; //quantas threads estão realizando operação de leitura
bool writing = false; //inicialmente não há escrita

typedef struct {
    int thread_id;
    int TAM;
    int pos;
    int size;
}Thread;

void *WriteData(void *threadid) {
    Thread tid = *((Thread *) threadid);
    int pos, value;
    while(1) {
        pthread_mutex_lock(&mutex);
        tid.pos = rand() % tid.TAM;
        while(writing) { //Caso haja operações de escrita, aguarde
            pthread_cond_wait(&write, &mutex);
        }
        //entrando na região crítica
        writing = true;
        value = rand()%tid.TAM;
        array[tid.pos] = value;
        printf("Valor %d adicionado na posicao %d\n", value, tid.pos);
        
        writing = false;
        pthread_cond_signal(&write); //acorda uma única thread de escrita
        pthread_cond_broadcast(&read); //acorda todas as threads de leitura que estão dependendo da variável read para prosseguir
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

void *ReadData(void *threadid) {
    Thread tid = *((Thread *) threadid);
    while(1) {
        tid.pos = rand() % tid.TAM;
        
        pthread_mutex_lock(&mutex1);
        while(writing) { //Caso haja operação de escrita, aguarde
            pthread_cond_wait(&read, &mutex1);
        }
        pthread_mutex_unlock(&mutex1);
    
        readers++;
        
        if(!writing) printf("Valor da leitura na posicao %d do array eh %d (Lido pela thread %d)\n", tid.pos, array[tid.pos], tid.thread_id); //Apenas ler caso não tenha escrita
        
        
        if(readers == tid.size) {
            readers = 0;
            pthread_cond_signal(&write); //Libera a escrita caso não haja leituras
        }
    }
    
    pthread_exit(NULL);
}


int main() {
    int N, M, rc, tam;
      
    cout << "Qual o valor N de threads leitoras?" << endl;
    cin >> N;
    
    cout << "Qual o valor M de threads escritoras?" << endl;
    cin >> M;
    
    cout << "Qual eh o tamanho do array?" << endl;
    cin >> tam;
    
    array = (int *) malloc(sizeof(int) * tam); 
    
    Thread *read[N]; 
    Thread *write[M]; //criando o vetor de ponteiros para a struct Thread
    
    pthread_t threads_read[N];
    pthread_t threads_write[M]; //criando o vetor de pthread_t para usar a biblioteca pthread.h
     
    for(int t = 0; t < M; t++) {
        write[t] = (Thread *) malloc(sizeof(Thread));
        write[t]->thread_id = t;
        write[t]->TAM = tam; 
        write[t]->size = M;
        
        rc = pthread_create(&threads_write[t], NULL, &WriteData, (void *) write[t]);
        
        if(rc) {
            cout <<"ERROR; return code is " << rc << endl;
            exit(-1);
        }
    }
    
    for(int t = 0; t < N; t++) {
        read[t] = (Thread *) malloc(sizeof(Thread));
        read[t]->thread_id = t;
        read[t]->TAM = tam;   
        read[t]->size = N;
        
        rc = pthread_create(&threads_read[t], NULL, &ReadData, (void *) read[t]);
        
        if(rc) {
            cout <<"ERROR; return code is " << rc << endl;
            exit(-1);
        }
    }
  
    for(int r = 0, w = 0; r < N && w < M ; r++, w++) {
        pthread_join(threads_read[r], NULL);
        pthread_join(threads_write[w], NULL);
    }
  
 
    return 0;
}
