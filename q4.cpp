#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

#define ii pair<int, int>

const int maxn = 1e3+10; //max map's size
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

void solve(int **map, int lim_x, int lim_y){
    for (int x = 0; x < lim_x; x++){
        for (int y = 0; y < lim_y; y++){
            if(map[x][y]){ //se for terra
                for(int adj = 0; adj < 4; adj++){
                    int ceil_adj_x = x + adj_x[adj];
                    int ceil_adj_y = y + adj_y[adj]; 

                    if(ceil_adj_x >= 0 && ceil_adj_x < lim_x && ceil_adj_y >= 0 && ceil_adj_y < lim_y){
                        if(map[ceil_adj_x][ceil_adj_y]){//se for terra
                            ii me = dsFind(x,y);
                            ii ceil_adj = dsFind(ceil_adj_x,ceil_adj_y);

                            if(me.first != ceil_adj.first || me.second != ceil_adj.second){
                                dsUnion(me, ceil_adj);
                            }
                        }
                    }
                }
            }
        }
        
    }
    
}

int main(){
    int N;
    int map[N][N];

    return 0;
}