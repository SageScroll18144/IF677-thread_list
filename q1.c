#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 10     

int count;
char decode[11];
char senha[] = "aeiouAEIOU";

typedef struct thread_id{
    char comparador;
    int char_t;
}T;

void *findChar(void *threadid){   
    
    T tid = *((T *)threadid);
    int flag = 0, x = 33;
    
    for(; tid.comparador || flag == 0;){
        tid.comparador = x;
        if(tid.comparador == senha[tid.char_t]){
            decode[tid.char_t] = tid.comparador;
            flag = 1;
            printf("caracter %d achado\n\n", tid.char_t);
            count++;
        }
        x++;
    }
    
    flag = 0;
    pthread_exit(NULL);
}


int main (int argc, char *argv[]){
    decode[10] = '\0';
    pthread_t threads[NUM_THREADS];
    T *taskids[NUM_THREADS];
    int rc;   
    int t;   

    for(t=0; t<NUM_THREADS; t++){   
        taskids[t] = (T *) malloc(sizeof(T)); 
        taskids[t]->char_t = t;
        int char_t = 33;
        rc = pthread_create(&threads[t], NULL, &findChar, (void *) taskids[t]);      
        if (rc){         
            printf("ERRO; código de retorno é %d\n", rc);         
            exit(-1);      
        }   
    }  
    
    for(int i=0; i<10000000; i++){}
  
   printf("%s\n", decode);
  
  pthread_exit(NULL);
}
