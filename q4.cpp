#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define ii pair<int, int>

const int maxn = 110; //max map_'s size
ii ds[maxn][maxn];//set of fathers
int adj_x[] = {0,0,-1,1};
int adj_y[] = {1,-1,0,0};

void dsBuild(){
    for (int i = 0; i < maxn; i++){
        for (int j = 0; j < maxn; j++){
            ds[i][j] = {i, j};
        }
    }    
}

ii dsFind(int x, int y){
    if(ds[x][y].first == x && ds[x][y].second == y) return {x, y};
    return dsFind(ds[x][y].first, ds[x][y].second);
}

void dsUnion(ii u, ii v){
    u = dsFind(u.first, u.second);
    v = dsFind(v.first, v.second);

    if((u.first*u.first + u.second*u.second) <  (v.first*v.first + v.second*v.second)){
        ii tmp = u;
        u = v;
        v = tmp;
    }

    ds[v.first][v.second] = u;
}

void solve(int **map_, int lim_x, int lim_y){
    for (int x = 0; x < lim_x; x++){
        for (int y = 0; y < lim_y; y++){
            if(map_[x][y]){ //se for terra
                for(int adj = 0; adj < 4; adj++){
                    int ceil_adj_x = x + adj_x[adj];
                    int ceil_adj_y = y + adj_y[adj]; 

                    if(ceil_adj_x >= 0 && ceil_adj_x < lim_x && ceil_adj_y >= 0 && ceil_adj_y < lim_y){
                        if(map_[ceil_adj_x][ceil_adj_y]){//se for terra
                            dsUnion({x,y}, {ceil_adj_x, ceil_adj_y});
                        }
                    }
                }
            }
        }
        
    }
    
}

int counter_fathers(int **map_, int lim_x, int lim_y){
    bool mark[lim_x][lim_y];
    memset(mark, false, sizeof(mark));

    for (int i = 0; i < lim_x; i++){
        for (int j = 0; j < lim_y; j++){
            ii dad = dsFind(i, j);
            if(map_[dad.first][dad.second]) mark[dad.first][dad.second] = true;
        }
    }  
    int counter_fathers = 0;
    for (int i = 0; i < lim_x; i++){
        for (int j = 0; j < lim_y; j++){
            if(mark[i][j]) counter_fathers++;
        }
    }

    return counter_fathers;
}

int main(){
    dsBuild();

    printf("- Informe o número de linhas e colunas: ");

    int lim_x, lim_y;
    scanf("%d%d", &lim_x, &lim_y);

    printf("- Informe a imagem do mapa:\n");

    int **map_ = (int **) malloc(lim_x * sizeof(int *));
    char component;
    for (int i = 0; i < lim_x; i++){
        map_[i] = (int *) malloc(lim_y * sizeof(int));
        for (int j = 0; j < lim_y; j++){
            scanf(" %c", &component);
            map_[i][j] = component - '0';
        }
    } 

    int N;
    printf("Informe o número N de Threads: ");
    scanf("%d", &N);


    solve(map_, lim_x, lim_y);
    printf("%d\n", counter_fathers(map_, lim_x, lim_y));

    for(int i = 0; i < lim_x; i++) free(map_[i]);
    free(map_);

    return 0;
}

/*
    Exemplo de entrada:

    - Informe o número de linhas e colunas: 5 7
    - Informe a imagem do mapa:
    1111000
    1110000
    0000100
    0000011
    0000111
    Informe o número N de Threads: 5

*/