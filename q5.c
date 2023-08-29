#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

int N, curr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool *array;

typedef struct thread_id{
    int numero;
    int comp;
}T;

void *primos(void *threadid){
    while(curr < N){
        pthread_mutex_lock(&mutex);
        
        T tid = *((T*) threadid);
        tid.numero = curr;
        while(array[tid.numero] == false && tid.numero < N){
            tid.numero++;
        }
        tid.comp = tid.numero;
        if(tid.numero == N){
            curr = N;
        }
        else{
            tid.comp = tid.comp + tid.numero;
            while(tid.comp < N){
                array[tid.comp] = false;
                tid.comp = tid.comp + tid.numero;
            }
            curr = tid.numero + 1;
        }
        
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

int main()
{
    int nt, rc;
    
    scanf("%d", &nt);
    scanf("%d", &N);
    
    pthread_t threads[nt];
    array = (bool*)malloc(sizeof(bool) * N);
    for(int i=0; i<N; i++) array[i] = true;
    
    array[0] = array[1] = false;
    curr = 2;
    
    T *task = (T*)malloc(sizeof(T));
    
    for(int i=0; i<nt; i++){
        rc = pthread_create(&threads[i], NULL, &primos, (void *) task);
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        } 
    }
    
    for(int i=0; i<nt; i++){
        pthread_join(threads[i], NULL);
    }
    
    
    for(int i=0; i<N; i++){
        if(array[i] == true){
            printf("%d\n", i);
        }
    }
    
    pthread_exit(NULL);
}
