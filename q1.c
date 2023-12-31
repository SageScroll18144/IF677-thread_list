#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n_threads_caso1 10 
#define n_threads_caso2 5
#define n_threads_caso3 1  

char decode1[11]; //usados para decodificar cada caso
char decode2[11];
char decode3[11];
char senha[] = "a!i!_45IOU"; //senha estatica

typedef struct thread_code{
    int thread_id;    //numero da thread
    char comparador;  //usado para comparar cm a senha
    int char_t;       //usado pra definir o indice do caracter buscado
}T;

//solução caso 1 (10 threads)
void *findChar1(void *threadid){   
    T tid = *((T *)threadid);
    int flag = 0; //flag para saber se achou o caractere
    int x = 33; //primeiro caractere possivel para uma senha dado na tabela ASCII
    tid.char_t = tid.thread_id;
    while(!flag){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode1[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}

//solução caso 2 (5 thhreads)
void *findChar2(void *threadid){    
    T tid = *((T *)threadid);
    int flag = 0; //flag para saber se achou o caractere
    int x = 33; //primeiro caractere possivel para uma senha dado na tabela ASCII
    tid.char_t = tid.thread_id;
    while(!flag){  //primeiro caracter da thread
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    x = 33;
    flag = 0;
    tid.char_t += 5;
    
    while(!flag){  //segundo caracter da thread
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}

//solução para o terceiro caso (1 thread)
void *findChar3(void *threadid){   
    for(int i=0; i<10; i++){
        T tid = *((T *)threadid);
        int flag = 0; //flag para saber se achou o caractere
        int x = 33; //primeiro caractere possivel para uma senha dado na tabela ASCII
        tid.char_t = i;
        while(!flag){
            tid.comparador = x;
            if(tid.comparador == senha[tid.char_t]){
                decode3[tid.char_t] = tid.comparador;
                flag = 1;
                printf("caracter %d achado pela thread %d\n", tid.char_t, (tid.thread_id + 1));
            }
            x++;
        }
        
        flag = 0;
    }
    pthread_exit(NULL);
}

int main (){
    int rc;   
    int t;
    double time1, time2, time3;
    
    printf("Caso 1 (10 threads):\n\n");
    
    clock_t comeco1 = clock(); //inicio da contagem do tempo do primeiro caso
    
    T *task1[n_threads_caso1];
    pthread_t threads1[n_threads_caso1];
    decode1[10] = '\0';
    
    for(t=0; t<n_threads_caso1; t++){   
        
        task1[t] = (T *) malloc(sizeof(T)); 
        task1[t]->thread_id = t;
        
        rc = pthread_create(&threads1[t], NULL, &findChar1, (void *) task1[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }
    
    for(t=0; t<n_threads_caso1; t++){
        pthread_join(threads1[t], NULL);
    }
    
    clock_t fim1 = clock(); //fim da contagem do tempo do primeiro caso
    
    time1 = (double) (fim1 - comeco1)/CLOCKS_PER_SEC;
    
    printf("\nTempo do Caso 1: %0.7f\n", time1);
  
    printf("Senha decodificada: %s\n\n", decode1);
   
    printf("Caso 2 (5 threads):\n\n");
   
    clock_t comeco2 = clock(); //inicio da contagem do tempo do segundo caso

    T *task2[n_threads_caso2];
    pthread_t threads2[n_threads_caso2];
    decode2[10] = '\0';

    for(t=0; t<n_threads_caso2; t++){   
        
        task2[t] = (T *) malloc(sizeof(T)); 
        task2[t]->thread_id = t;
        
        rc = pthread_create(&threads2[t], NULL, &findChar2, (void *) task2[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }

    for(t=0; t<n_threads_caso2; t++){
        pthread_join(threads2[t], NULL);
    }

    clock_t fim2 = clock(); //fim da contagem do tempo do segundo caso

    time2 = (double) (fim2 - comeco2)/CLOCKS_PER_SEC;

    printf("\nTempo do Caso 2: %0.7f\n", time2);

    printf("Senha decodificada: %s\n\n", decode3);

    printf("Caso 3 (1 threads):\n");

    clock_t comeco3 = clock(); //inicio da contagem do tempo do terceiro caso

    T *task3[n_threads_caso3];
    pthread_t threads3[n_threads_caso3];
    decode3[10] = '\0';

    for(t=0; t<n_threads_caso3; t++){   
        
        task3[t] = (T *) malloc(sizeof(T)); 
        task3[t]->thread_id = t;
        
        rc = pthread_create(&threads3[t], NULL, &findChar3, (void *) task3[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }

    for(t=0; t<n_threads_caso3; t++){
        pthread_join(threads3[t], NULL);
    }

    clock_t fim3 = clock(); //fim da contagem do tempo do terceiro caso

    time3 = (double) (fim3 - comeco3)/CLOCKS_PER_SEC;

    printf("\nTempo do Caso 3: %0.7f\n", time3);
    printf("Senha decodificada: %s\n\n", decode3);

    printf("tempo de execucao 10 threads : %0.7f\n", time1);
    printf("tempo de execucao 5 threads : %0.7f\n", time2);
    printf("tempo de execucao 1 thread : %0.7f\n", time3);

    pthread_exit(NULL);
}
