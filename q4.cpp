#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define ii pair<int, int>

const int maxn = 110; //max map_'s size
ii ds[maxn][maxn];//set of fathers
int adj_x[] = {0,0,-1,1,1,-1,1,-1};
int adj_y[] = {1,-1,0,0,1,-1,-1,1};

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

void solve(int **map_, int lim_inf_x, int lim_inf_y, int lim_sup_x, int lim_sup_y){
    for (int x = lim_inf_x; x < lim_sup_x; x++){
        for (int y = lim_inf_y; y < lim_sup_y; y++){
            if(map_[x][y]){ //se for terra
                for(int adj = 0; adj < 8; adj++){
                    int ceil_adj_x = x + adj_x[adj];
                    int ceil_adj_y = y + adj_y[adj]; 

                    if(ceil_adj_x >= 0 && ceil_adj_x < lim_sup_x && ceil_adj_y >= 0 && ceil_adj_y < lim_sup_y){
                        if(map_[ceil_adj_x][ceil_adj_y]){//se for terra
                            dsUnion({x,y}, {ceil_adj_x, ceil_adj_y});
                        }
                    }
                }
            }
        }
        
    }
    
}

int counter_fathers(int **map_, int lim_inf_x, int lim_inf_y, int lim_sup_x, int lim_sup_y){
    bool mark[lim_sup_x][lim_sup_y];
    memset(mark, false, sizeof(mark));

    for (int i = lim_inf_x; i < lim_sup_x; i++){
        for (int j = lim_inf_y; j < lim_sup_y; j++){
            ii dad = dsFind(i, j);
            if(map_[dad.first][dad.second]) mark[dad.first][dad.second] = true;
        }
    }  
    int counter_fathers = 0;
    for (int i = lim_inf_x; i < lim_sup_x; i++){
        for (int j = lim_inf_y; j < lim_sup_y; j++){
            if(mark[i][j]) counter_fathers++;
        }
    }

    return counter_fathers;
}

ii findFactors(int N) {
    int ans_a = -1;
    int ans_b = -1;
    
    int a = 1, mid = N+1;
    while (a * a <= N) {
        if (N % a == 0) {
            int b = N / a;
            if(a <= N && b <= N && 2*mid > (a + b))  {
                ans_a = a;
                ans_b = b;

                mid = (a + b) / 2;
            }
        }
        a++;
    }
    return {ans_a,ans_b};
}

int main(){
    //build DSU
    dsBuild();

    //Entrada do sistema
    printf("- Informe o número de linhas e colunas: ");

    int lim_x, lim_y; // linha e coluna
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

    //Pre definição das threads
    ii NFactors = findFactors(N);

    int A = NFactors.first, B = NFactors.second;

    //condição aritmética para atender casos geometricamentes consonantes
    if(A > B && lim_x < lim_y){
        //troca A por B e B por A
        A += B;
        B = A - B;
        A -= B;
    }

    int Q1 = (lim_x / A) + 1, Q2 = (lim_y / B) + 1, idx = 0;
    
    ii pairs_lim_x[A];
    for(int i = 0; i < lim_x; i += Q1){
        pairs_lim_x[idx] = {i, i + Q1 - 1};
        idx++;
    }
    pairs_lim_x[idx - 1].second = (lim_x - 1);
    
    idx = 0;
    ii pairs_lim_y[B];
    for(int i = 0; i < lim_y; i += Q2){
        pairs_lim_y[idx] = {i, i + Q2 - 1};
        idx++;
    }
    pairs_lim_y[idx - 1].second = (lim_y - 1);    

    for(int i=0;i<A;i++){
        printf("{%d, %d}\n", pairs_lim_x[i].first, pairs_lim_x[i].second);
    }
    printf("\n");
    for(int i=0;i<B;i++){
        printf("{%d, %d}\n", pairs_lim_y[i].first, pairs_lim_y[i].second);
    }

    //without thread
    solve(map_, 0, 0, lim_x, lim_y);
    printf("%d\n", counter_fathers(map_, 0, 0, lim_x, lim_y));

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