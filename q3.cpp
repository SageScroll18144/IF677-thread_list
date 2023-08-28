#include <iostream>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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
        while(readers > 0 || writing) { //Caso haja operações de leitura ou de escrita, aguarde
            pthread_cond_wait(&write, &mutex);
        }
        //entrando na região crítica
        writing = true;
        cout << "Digite o valor para ser adicionado no array pela thread " << tid.thread_id << " na posicao " << tid.pos << endl;
        cin >> value;
        array[tid.pos] = value;
        
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
        
        while(writing) { //Caso haja operação de escrita, aguarde
            pthread_cond_wait(&read, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    
        readers++;
        
        printf("Valor da leitura na posicao %d do array eh %d (Lido pela thread %d)\n", tid.pos, array[tid.pos], tid.thread_id);
        
        
        if(readers == tid.size) {
            readers = 0;
            pthread_cond_signal(&write); //Libera a escrita caso não haja leituras
        }
    }
    
    pthread_exit(NULL);
}


int main() {
    int N, M, rc, tam;
    int flag = 1;
    
    while(flag) { //loop para checar a validade da entrada
        
        cout << "Qual o valor N de threads leitoras?" << endl;
        cin >> N;
        
        cout << "Qual o valor M de threads escritoras?" << endl;
        cin >> M;
        
        int bigger = max(M, N);
        
        cout << "Qual eh o tamanho do array?" << endl;
        cin >> tam;
        
        if(bigger <= tam) flag = 0; //Isso é feito para que o tamanho do array seja maior que N e M
        
        else {
            cout << "Entrada invalida" << endl << "Digite novamente os valores de N, M e o tamanho do array" << endl << endl;
        }
    }
    
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
