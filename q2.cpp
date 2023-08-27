#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

typedef struct{
    int ID;
    int a, b, c;
}resource;

const int maxn = 110;

vector<int> graph[maxn];
bool visited[maxn];

bool DFS_find_cycle(int node){
    visited[node] = true;
    for(auto son : graph[node]){
        if(!visited[son]) return DFS_find_cycle(son);
        else if(visited[son]) return true;
    }
    return false;
}

int main(){
    
    int N, M, adj_node;
    printf("Informe a quantidade de nós do grafo: ");
    scanf("%d", &N);

    for(int i = 0; i < N; i++){
        printf("Informe a quantidade de operações para o processo/recurso %d: ", i+1);
        scanf("%d", &M);
        for(int j = 0; j < M; j++){
            printf("Este processo requisita ou este recurso tem posse (de): ");
            scanf("%d", &adj_node);

            graph[i].push_back(adj_node);
        }
    }

    printf("Modelado o Grafo com Sucesso!\n\tBuscando deadlock...\n\n");

    printf("%d\n", DFS_find_cycle(0));

    return 0;
}