#include <cstdio>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

enum State {NONE, DEAD, ACTIVATE, WAIT};
enum DIR {UP, RIGHT, DOWN, LEFT};
const int dx[] = {0, 1, 0, -1};
const int dy[] = {-1, 0, 1, 0};
int N, M, K;
int global_time;

struct Cell
{
    Cell(int _i_, int _j_, int _power_, State st) : i(_i_), j(_j_), power(_power_), state(st) {
        life = _power_ + global_time;
    }
    Cell(int _i_, int _j_) : i(_i_), j(_j_) {
        power = 0;
        life = 0;
        state = NONE;
    }
    int i;
    int j;
    int power;
    int life;
    State state;
};

vector<Cell> waits;
vector<Cell> actives;
set<pair<int, int>> placed;

bool CellGreater(const Cell& a, const Cell& b)
{
    return a.power > b.power;
}

void regist(int i, int j, int power)
{
    Cell c(i, j, power, WAIT);
    waits.push_back(c);
    placed.insert(make_pair(i, j));
}

void activate()
{
    for(int i = 0; i < waits.size(); ++i)
    {
        Cell c = waits[i];
        if(c.state == WAIT && c.life == global_time)
        {
            c.state = waits[i].state = ACTIVATE;
            actives.push_back(c);
        }
    }
    for(vector<Cell>::iterator it = waits.begin(); it != waits.end();)
    {
        if(it->state == ACTIVATE)
        {
            it = waits.erase(it);
            continue;
        }
        ++it;
    }
    sort(actives.begin(), actives.end(), CellGreater);
}

void die()
{
    for(int i = 0; i < actives.size(); ++i)
    {
        Cell c = actives[i];
        if(c.state == ACTIVATE && c.life + 1 == global_time)
        {
            for(int d = UP; d <= LEFT; ++d)
            {
                int ni = c.i + dy[d], nj = c.j + dx[d];
                pair<int, int> pos;
                pos.first = ni;
                pos.second = nj;
                if(placed.find(pos) == placed.end())
                {
                    regist(ni, nj, c.power);
                }
            }
            c.state = actives[i].state = NONE;
        }
        if(c.state == NONE && c.life + c.power == global_time)
        {
            actives[i].state = DEAD;
        }
    }
    sort(waits.begin(), waits.end(), CellGreater);
    for(vector<Cell>::iterator it = actives.begin(); it != actives.end();)
    {
        if(it->state == DEAD) {
            it = actives.erase(it);
            continue;
        }
        ++it;
    }
}

void initialize()
{
    global_time = 0;
    waits.clear();
    actives.clear();
    placed.clear();
}

int main()
{
    //freopen("sample_input.txt", "r", stdin);
    int T;  scanf("%d", &T);
    for(int t = 1; t <= T; ++t)
    {
        initialize();
        scanf("%d%d%d", &N, &M, &K);
        int power;
        for(int i = 0 ; i < N; ++i) {
            for(int j = 0; j < M; ++j) {
                scanf("%d", &power);
                if(power == 0)  continue;
                regist(i, j, power);
            }
        }
        for(global_time = 1; global_time <= K ; ++global_time)
        {
            activate();
            die();
        }
        printf("#%d %d\n", t, waits.size() + actives.size());
    }
    return 0;
}