#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

const int maxn = 110;

vector<int> graph[maxn]; //grafo por lista de adjacência 
bool visited[maxn]; //vetor de visitados

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex, pois todas as threads acessam o grafo

bool flag_ans = false; //flag de resposta para saber se achamos o ciclo

bool DFS_find_cycle(int node){
    visited[node] = true;
    for(auto son : graph[node]){
        if(!visited[son]) return DFS_find_cycle(son);
        else if(visited[son]) return true;
    }
    return false;
}


void *solve(void * args_){
    int *node = (int *) args_;

    pthread_mutex_lock(&mutex);

    for(int i = 0; i < maxn; i++) visited[i] = false;
    bool ans_tmp = DFS_find_cycle(*node);
    flag_ans = (ans_tmp) ? ans_tmp : flag_ans;

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);

}

int main(){
    
    int N, M, adj_node;
    printf("Informe a quantidade de nós do grafo: ");
    scanf("%d", &N);

    for(int i = 0; i < N; i++){
        printf("Informe a quantidade de operações para o processo/recurso %d: ", i+1);
        scanf("%d", &M);
        for(int j = 0; j < M; j++){
            printf("\n(Infome um valor entre [1,NUM NODES]).\tEste processo %d requisita ou este recurso tem posse (de): ", i+1);
            scanf("%d", &adj_node);

            graph[i].push_back(adj_node - 1);
        }
    }

    printf("\nModelado o Grafo com Sucesso!\n\tBuscando deadlock...\n\n");

    pthread_t list_threads[N];
    int t, rc;
    int *idx_args;
    for(t = 0; t < N; t++) {
        idx_args = (int *) malloc(sizeof(int));
        idx_args = &t;
        
        rc = pthread_create(&list_threads[t], NULL, solve, (void *)idx_args);
        
        if(rc){
            printf("ERRO; código de retorno é %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < N; t++) pthread_join(list_threads[t], NULL);

    printf("%s\n", (flag_ans) ? "\n - Deadlock encontrado." : "\n - Não foi encontrado Deadlock.");

    pthread_exit(NULL);

    return 0;
}