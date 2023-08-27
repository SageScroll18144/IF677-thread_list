#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define n_threads_caso1 10 
#define n_threads_caso2 5
#define n_threads_caso3 1  


char decode1[11];
char decode2[11];
char decode3[11];
char senha[] = "a!i!_45IOU";

typedef struct thread_code{
    int thread_id;
    char comparador;
    int char_t;
}T;

void *findChar2(void *threadid){   
    
    T tid = *((T *)threadid);
    int flag = 0, x = 33;
    tid.char_t = tid.thread_id;
    for(; flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    
    x = 33;
    flag = 0;
    tid.char_t = tid.char_t + 5;
    
    for(;flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}

void *findChar1(void *threadid){   
    
    T tid = *((T *)threadid);
    int flag = 0, x = 33;
    tid.char_t = tid.thread_id;
    for(;flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode1[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, (tid.thread_id + 1));
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}


void *findChar3(void *threadid){   
    for(int i=0; i<10; i++){
        T tid = *((T *)threadid);
        int flag = 0, x = 33;
        tid.char_t = i;
        for(; tid.comparador || flag == 0;){
            tid.comparador = x;
            if(tid.comparador == senha[tid.char_t]){
                decode3[tid.char_t] = tid.comparador;
                flag = 1;
                printf("caracter %d achado pela thread %d\n\n", tid.char_t, (tid.thread_id + 1));
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
    
    clock_t comeco1 = clock();
    
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
    
    clock_t fim1 = clock();
    
    time1 = (double) (fim1 - comeco1)/CLOCKS_PER_SEC;
    
    printf("%0.7f\n\n", time1);
  
   printf("%s\n\n", decode1);
   
   
   printf("Caso 2 (5 threads):\n\n");
   
   clock_t comeco2 = clock();
    
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
    
    clock_t fim2 = clock();
    
    time2 = (double) (fim2 - comeco2)/CLOCKS_PER_SEC;
    
    printf("%0.7f\n\n", time2);
    
    printf("%s\n\n", decode3);
    
    printf("Caso 3 (1 threads):\n\n");
    
    clock_t comeco3 = clock();
    
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
    
    clock_t fim3 = clock();
    
    time3 = (double) (fim3 - comeco3)/CLOCKS_PER_SEC;
    
    printf("%s\n\n", decode3);
    
    printf("tempo de execucao 10 threads : %0.7f\n\n", time1);
    printf("tempo de execucao 5 threads : %0.7f\n\n", time2);
    printf("tempo de execucao 1 thread : %0.7f\n\n", time3);
  
    pthread_exit(NULL);
}
