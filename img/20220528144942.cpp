#include<bits/stdc++.h>
using namespace std;
#define LL long long
#define INF 0x3f3f3f3f3f3f3f3f
struct edge{
    int to;
    LL weight;
    int pos;//记录改变在节点存储边的vector中的序号
};
const int N = 2e4 + 5, M = 2e5 + 5;
vector<edge> G[N];
int level[N];
int pos_num[N];
void add(int from, int to, LL w)
{
    G[from].push_back({to, w,(int) G[to].size()});
    G[to].push_back({from, 0, (int) G[from].size() - 1});//这是为了求最大流记录记录from流向to的流量，方便回溯，弥补最优解
}
LL gety(int x, int a, int b, int c)
{
    return a * x * x + b * x + c;
}
LL cal(int l, int r, int a, int b, int c)
{
    if(l > r) return 0;
    double x = - b / 2.0 / a;
    if(a >= 0){
        return max(gety(l, a, b, c), gety(r, a, b, c));
    }
    else{
        if(l > x)
            return gety(l, a, b, c);
        else if(r < x)
            return gety(r, a, b, c);
        else
            return max(gety(floor(x), a, b, c), gety(ceil(x), a, b, c));
    }
}
bool bfs()
{
    memset(level, -1, sizeof(level));
    queue<int> q;
    level[0] = 0;
    q.push(0);
    while(! q.empty())
    {
        int u = q.front();
        q.pop();
        for(int i = 0; i < G[u].size(); i ++){
            edge &e = G[u][i];
            if(e.weight > 0 && level[e.to] < 0){
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }
}
LL dfs(int u, int t, LL f)
{
    if(u == t) return f;
    for(int &i = pos_num[u]; i < G[u].size(); i ++){
        edge &e = G[u][i];
        if(e.weight > 0 && level[u] + 1 == level[e.to]){
            LL w = dfs(e.to, t, min(f, e.weight));
            if(w > 0){
                e.weight -= w;
                G[e.to][e.pos].weight += w;
                return w;
            }
        }
    }
    return 0;
}
LL max_flow(int s, int t)
{
    LL flow = 0;
    while(1){
        bfs();
        if(level[t] < 0) return flow;
        memset(pos_num, 0, sizeof(pos_num));
        LL f = 0;
        while((f = dfs(s, t, INF)) > 0)
            flow += f;
    }
    return 0;
}
int main()
{
    int n, m;
    cin>>n>>m;
    LL weight_sum = 0;
    int T = 2 * n + 1;//汇聚节点
    for(int i = 1; i <= n; i ++){
        int l, r, a, b, c;
        cin>>l>>r>>a>>b>>c;
        //计算权值，不包括边界点
        LL w1 = cal(l + 1, r - 1, a, b, c);
        //计算边界点
        LL w2 = max(gety(l, a, b, c), gety(r, a, b, c)) - w1;
        //cout<<"w1:"<<w1<<"w2:"<<w2<<endl;
        if(w1 > 0){
            weight_sum += w1;
            add(0, i, w1);
        }
        else{
            add(i, T, -w1);
        }
        if(w2 > 0){
            weight_sum += w2;
            add(0, i + n, w2);
        }
        else{
            add(i + n, T, -w2);
        }
        add(i + n, i, INF);
    }
    for(int i = 0; i < m; i ++){
        int z, x, y;
        cin>>z>>x>>y;
        if(z == 1){
            add(y, x, INF);
        }
        else {
            add(y + n, x, INF);
        }
    }
    cout<<weight_sum - max_flow(0, T)<<endl;
}


