#include <bits/stdc++.h>

using namespace std;

const int maxn = 1e3+10; //pior caso

vector<int> graph[maxn];
bool visited[maxn];

bool DFS_find_cycle(int node){
    visited[node] = true;
    for(auto son : graph[node]){
        if(!visited[son]) DFS_find_cycle(son);
        else if(visited[son]) return true;
    }
    return false;
}

int main(){
    
    //talvez trocar o tipo do grafo, de int para p_thread 

    return 0;
}