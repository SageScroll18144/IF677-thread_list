#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

int N, curr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool *array; //array de bool para usar em todas as threads sem precisar passar pela função

typedef struct thread_id{
    int numero;
    int comp;
}T;

void *primos(void *threadid){ //funcao das threads
    while(curr < N){
        pthread_mutex_lock(&mutex);//mutex para que cada thread decida seu numero primo a se analisar de cada vez
        
        T tid = *((T*) threadid);
        tid.numero = curr;
        while(array[tid.numero] == false && tid.numero < N){ //busca o primeiro numero primo ainda não visitado
            tid.numero++;
        }
        tid.comp = tid.numero;
        if(tid.numero == N){ //caso ja tenha visitado todos os numeros primos, coloca o curr=N para fechar as threads
            curr = N;
        }
        else{ //setar todos os múltiplos do número como false
            tid.comp = tid.comp + tid.numero;
            while(tid.comp < N){
                array[tid.comp] = false;
                tid.comp = tid.comp + tid.numero;
            }
            curr = tid.numero + 1;
        }
        
        pthread_mutex_unlock(&mutex); //libera que outras threads possam acessar essa região
    }
    
    pthread_exit(NULL);
}

int main()
{
    int nt, rc;
    printf("número de threads: ");
    scanf("%d", &nt); //numero de threads
    printf("até que numero devemos analisar? ");
    scanf("%d", &N);  //até que numero vai a lista
    
    pthread_t threads[nt];
    array = (bool*)malloc(sizeof(bool) * N);
    for(int i=0; i<N; i++) array[i] = true; //setar todo mundo inicialmente como true
    
    array[0] = array[1] = false; //0 e 1 false
    curr = 2; //curr sempre vai ser o proximo caracter a ser lido
    
    T *task = (T*)malloc(sizeof(T));
    
    for(int i=0; i<nt; i++){
        rc = pthread_create(&threads[i], NULL, &primos, (void *) task); //criando as threads
        if (rc){     //checa se ouve erro na criacao    
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        } 
    }
    
    for(int i=0; i<nt; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("primos:\n\n");
    
    for(int i=0; i<N; i++){//printa todos os primos
        if(array[i] == true){
            printf("%d\n", i);
        }
    }
    
    pthread_exit(NULL);
}
