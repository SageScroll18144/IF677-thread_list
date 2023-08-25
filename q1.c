#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS_CASO1 10 
#define NUM_THREADS_CASO2 5
#define NUM_THREADS_CASO3 1  

int count;
char decode1[11];
char decode2[11];
char decode3[11];
char senha[] = "aei!_45IOU";

typedef struct thread_code{
    int thread_id;
    char comparador;
    int char_t;
}T;

void *findChar2(void *threadid){   
    
    T tid = *((T *)threadid);
    int flag = 0, x = 33;
    tid.char_t = tid.thread_id;
    for(; tid.comparador || flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, tid.thread_id);
            count++;
        }
        x++;
    }
    
    
    x = 33;
    flag = 0;
    tid.char_t = tid.char_t + 5;
    
    for(; tid.comparador || flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode2[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, tid.thread_id);
            count++;
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
    for(; tid.comparador || flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode1[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado pela thread %d\n\n", tid.char_t, tid.thread_id);
            count++;
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}


int main (int argc, char *argv[]){
    int rc;   
    int t;   
    
    printf("Caso 1 (10 threads):\n\n");
    
    T *taskids1[NUM_THREADS_CASO1];
    pthread_t threads1[NUM_THREADS_CASO1];
    decode1[10] = '\0';
    
    for(t=0; t<NUM_THREADS_CASO1; t++){   
        
        taskids1[t] = (T *) malloc(sizeof(T)); 
        taskids1[t]->thread_id = t;
        
        rc = pthread_create(&threads1[t], NULL, &findChar1, (void *) taskids1[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }
    
    for(t=0; t<NUM_THREADS_CASO1; t++){
        pthread_join(threads1[t], NULL);
    }
    
  
   printf("%s\n\n", decode1);
   
   
   printf("Caso 2 (5 threads):\n\n");
    
    T *taskids2[NUM_THREADS_CASO2];
    pthread_t threads2[NUM_THREADS_CASO2];
    decode2[10] = '\0';
    
    for(t=0; t<NUM_THREADS_CASO2; t++){   
        
        taskids2[t] = (T *) malloc(sizeof(T)); 
        taskids2[t]->thread_id = t;
        
        rc = pthread_create(&threads2[t], NULL, &findChar2, (void *) taskids2[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }
    
    for(t=0; t<NUM_THREADS_CASO2; t++){
        pthread_join(threads2[t], NULL);
    }
    
    printf("%s\n\n", decode2);
  
    pthread_exit(NULL);
}
