#include <iostream>
#include <utility>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

#define ii pair<int, int>

typedef struct{
    int lim_inf_x;
    int lim_inf_y;
    int lim_sup_x;
    int lim_sup_y;
}arguments_for_solve;

const int maxn = 110; //max map_'s size
ii ds[maxn][maxn];//set of fathers
int map_[maxn][maxn];
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

    //criterio arbitrário de união: O lider de cada conjunto é o que tem a maior distancia cartesiana da origem
    if((u.first*u.first + u.second*u.second) <  (v.first*v.first + v.second*v.second)){
        ii tmp = u;
        u = v;
        v = tmp;
    }

    ds[v.first][v.second] = u;
}

void *solve(void * args_){
    arguments_for_solve *transform_args = (arguments_for_solve *) args_;
    
    int lim_inf_x = transform_args->lim_inf_x;
    int lim_inf_y = transform_args->lim_inf_y;
    int lim_sup_x = transform_args->lim_sup_x;
    int lim_sup_y = transform_args->lim_sup_y;

    for (int x = lim_inf_x; x <= lim_sup_x; x++){
        for (int y = lim_inf_y; y <= lim_sup_y; y++){
            if(map_[x][y]){ //se for terra
                for(int adj = 0; adj < 8; adj++){
                    int ceil_adj_x = x + adj_x[adj];
                    int ceil_adj_y = y + adj_y[adj]; 

                    //além de realizar a união com os elementos pertencente ao campo
                    if(ceil_adj_x >= lim_inf_x && ceil_adj_x <= lim_sup_x && ceil_adj_y >= lim_inf_y && ceil_adj_y <= lim_sup_y){
                        if(map_[ceil_adj_x][ceil_adj_y]){//se for terra
                            dsUnion({x,y}, {ceil_adj_x, ceil_adj_y});
                        }
                    }
                }
            }
        }
        
    }
    pthread_exit(NULL);
}

int counter_fathers(int lim_inf_x, int lim_inf_y, int lim_sup_x, int lim_sup_y){
    
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
    
    int a = 1, mid = N+100000;
    while (a * a <= N) {
        if (N % a == 0) {
            int b = N / a;
            if(mid > (a + b))  {
                ans_a = a;
                ans_b = b;

                mid = (a + b);
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

    char component;
    for (int i = 0; i < lim_x; i++){
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

    //aqui começa threads
    pthread_t list_threads[N];
    arguments_for_solve **args_ = (arguments_for_solve **) malloc(sizeof(arguments_for_solve *) * N);
            
    int t = 0, rc;
    for (int i = 0; i < A; i++){
        for (int j = 0; j < B; j++){
            args_[t] = (arguments_for_solve *) malloc(sizeof(arguments_for_solve));

            args_[t]->lim_inf_x = pairs_lim_x[i].first;
            args_[t]->lim_inf_y = pairs_lim_y[j].first;;
            args_[t]->lim_sup_x = pairs_lim_x[i].second;
            args_[t]->lim_sup_y = pairs_lim_y[j].second;
            
            rc = pthread_create(&list_threads[t], NULL, solve, (void *) args_[t]);
            if(rc){         
                printf("ERRO; código de retorno é %d\n", rc);         
                exit(-1);      
            } 
            t++;

        }
        
    }

    for(t = 0; t < N; t++) pthread_join(list_threads[t], NULL);

    // Repete o algoritmo percorrendo toda matriz realizando a união das fronteiras. Uma vez que o mapa converge para uma imagem só de lideres
    for (int x = 0; x < lim_x; x++){
        for (int y = 0; y < lim_y; y++){
            if(map_[x][y]){ //se for terra
                for(int adj = 0; adj < 8; adj++){
                    int ceil_adj_x = x + adj_x[adj];
                    int ceil_adj_y = y + adj_y[adj]; 

                    if(ceil_adj_x >= 0 && ceil_adj_x <= lim_x && ceil_adj_y >= 0 && ceil_adj_y <= lim_y){
                        if(map_[ceil_adj_x][ceil_adj_y] && dsFind(x, y) != dsFind(ceil_adj_x, ceil_adj_y)){//se for terra
                            dsUnion({x,y}, {ceil_adj_x, ceil_adj_y});
                        }
                    }
                }
            }
        }
        
    }

    //Saida do programa
    printf("O seu mapa têm %d ilhas!\n", counter_fathers(0, 0, lim_x, lim_y));

    free(args_);

    pthread_exit(NULL);
}

/*
    Exemplo:

    *entrada*
    - Informe o número de linhas e colunas: 5 7
    - Informe a imagem do mapa:
    1111000
    1110000
    0000000
    0010011
    0000111
    Informe o número N de Threads: 4
    *saida*
    O seu mapa têm 3 ilhas!

    *entrada
    - Informe o número de linhas e colunas: 5 7
    - Informe a imagem do mapa:
    1111000
    1110000
    0000000
    0010011
    0000111
    Informe o número N de Threads: 8
    *saida*
    O seu mapa têm 3 ilhas!

*/
