#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

typedef struct{
    int ID;

}process;

const int maxn = 110;

vector<process> graph[maxn];
bool visited[maxn];

bool DFS_find_cycle(process node){
    visited[node.ID] = true;
    for(auto son : graph[node.ID]){
        if(!visited[son.ID]) DFS_find_cycle(son);
        else if(visited[son.ID]) return true;
    }
    return false;
}

int main(){
    
    //talvez trocar o tipo do grafo, de int para p_thread 

    return 0;
}