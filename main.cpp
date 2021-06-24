#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define ALL(v) (v).begin(), (v).end()
using ll = long long;
using T = tuple<double, int, int>;
using P = pair<int, int>;

constexpr int N = 30;
constexpr int Q = 1000;
constexpr double C = 500;

double hh[N][N - 1], vv[N - 1][N];
int cnth[N][N - 1], cntv[N - 1][N];

double H[N], V[N];
int cntH[N], cntV[N];

double r = 0.5;

struct XorShift {
    unsigned int x, y, z, w, t;

    XorShift(int seed) {
        mt19937 rnd(seed);
        x = rnd();
        y = rnd();
        z = rnd();
        w = rnd();
        t = 1;
    }

    int rand() {
        t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w & 0x7fffffff;
    }
} rnd(rand());

void init() {
    fill(hh[0], hh[N], 5000);
    fill(vv[0], vv[N - 1], 5000);
    fill(H, H + N, 5000);
    fill(V, V + N, 5000);
}

vector<P> solve(int sx, int sy, int tx, int ty) {
    double h[N][N - 1], v[N - 1][N];
    rep(x, N) {
        rep(y, N - 1) {
            h[x][y] = max(
                H[x] * r + hh[x][y] * (1 - r) - (cnth[x][y] == 0 ? 1500 : 0) + C,
                0.0);
        }
    }
    rep(x, N - 1) {
        rep(y, N) {
            v[x][y] = max(
                V[y] * r + vv[x][y] * (1 - r) - (cntv[x][y] == 0 ? 1500 : 0) + C,
                0.0);
        }
    }
    double dist[N][N];
    fill(dist[0], dist[N], DBL_MAX);
    priority_queue<T, vector<T>, greater<T>> que;
    P prev[N][N];
    fill(prev[0], prev[N], P(-1, -1));
    dist[sx][sy] = 0;
    que.emplace(0, sx, sy);
    while (!que.empty()) {
        double t;
        int x, y;
        tie(t, x, y) = que.top();
        que.pop();
        if (dist[x][y] < t) continue;
        if (x > 0 && dist[x - 1][y] > dist[x][y] + v[x - 1][y]) {
            dist[x - 1][y] = dist[x][y] + v[x - 1][y];
            prev[x - 1][y] = P(x, y);
            que.emplace(dist[x - 1][y], x - 1, y);
        }
        if (x + 1 < N && dist[x + 1][y] > dist[x][y] + v[x][y]) {
            dist[x + 1][y] = dist[x][y] + v[x][y];
            prev[x + 1][y] = P(x, y);
            que.emplace(dist[x + 1][y], x + 1, y);
        }
        if (y > 0 && dist[x][y - 1] > dist[x][y] + h[x][y - 1]) {
            dist[x][y - 1] = dist[x][y] + h[x][y - 1];
            prev[x][y - 1] = P(x, y);
            que.emplace(dist[x][y - 1], x, y - 1);
        }
        if (y + 1 < N && dist[x][y + 1] > dist[x][y] + h[x][y]) {
            dist[x][y + 1] = dist[x][y] + h[x][y];
            prev[x][y + 1] = P(x, y);
            que.emplace(dist[x][y + 1], x, y + 1);
        }
    }
    int x = tx, y = ty;
    vector<P> res;
    res.emplace_back(x, y);
    while (prev[x][y].first != -1) {
        tie(x, y) = prev[x][y];
        res.emplace_back(x, y);
    }
    reverse(ALL(res));
    return res;
}

void outputEdge(int x, int y, int nx, int ny) {
    if (nx == x - 1) {
        cout << 'U';
    } else if (nx == x + 1) {
        cout << 'D';
    } else if (ny == y - 1) {
        cout << 'L';
    } else {
        cout << 'R';
    }
}

void output(vector<P> &ans) {
    rep(i, ans.size() - 1) {
        outputEdge(ans[i].first, ans[i].second, ans[i + 1].first,
                   ans[i + 1].second);
    }
    cout << endl;
}

double lenEdge(int x, int y, int nx, int ny) {
    if (nx == x - 1) {
        return V[y] * r + vv[x - 1][y] * (1 - r);
    } else if (nx == x + 1) {
        return V[y] * r + vv[x][y] * (1 - r);
    } else if (ny == y - 1) {
        return H[x] * r + hh[x][y - 1] * (1 - r);
    } else {
        return H[x] * r + hh[x][y] * (1 - r);
    }
}

void modifyEdge(int x, int y, int nx, int ny, double len) {
    if (nx == x - 1) {
        V[y] = V[y] * cntV[y] / (cntV[y] + 1) + len / (cntV[y] + 1);
        cntV[y]++;
        vv[x - 1][y] = len;
        cntv[x - 1][y]++;
    } else if (nx == x + 1) {
        V[y] = V[y] * cntV[y] / (cntV[y] + 1) + len / (cntV[y] + 1);
        cntV[y]++;
        vv[x][y] = len;
        cntv[x][y]++;
    } else if (ny == y - 1) {
        H[x] = H[x] * cntH[x] / (cntH[x] + 1) + len / (cntH[x] + 1);
        cntH[x]++;
        hh[x][y - 1] = len;
        cnth[x][y - 1]++;
    } else {
        H[x] = H[x] * cntH[x] / (cntH[x] + 1) + len / (cntH[x] + 1);
        cntH[x]++;
        hh[x][y] = len;
        cnth[x][y]++;
    }
}

double modify(vector<P> &ans, double b) {
    vector<double> len;
    double res = 0;
    rep(i, ans.size() - 1) {
        len.emplace_back(lenEdge(ans[i].first, ans[i].second, ans[i + 1].first,
                                 ans[i + 1].second));
        res += len.back();
    }
    rep(i, ans.size() - 1) {
        modifyEdge(ans[i].first, ans[i].second, ans[i + 1].first,
                   ans[i + 1].second, b * len[i] / res);
    }
    return res;
}

signed main() {
    init();
    rep(k, Q) {
        int sx, sy, tx, ty;
        cin >> sx >> sy >> tx >> ty;
        auto ans = solve(sx, sy, tx, ty);
        output(ans);
        double b;
        cin >> b;
        double expectb = modify(ans, b);
        double score = 350 - abs((expectb - b) / (ans.size() - 1));
        if (score < 0) {
            r += (rnd.rand() % (int)1e9 - 5e8) * (Q * 1.5 - k) / (2e10 * Q);
            if (r < 0) r = 0;
            if (r > 1) r = 1;
        }
    }
    return 0;
}