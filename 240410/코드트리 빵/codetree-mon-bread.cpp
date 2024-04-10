#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

int n; // 격자의 크기; 2 <= n <= 15
int m; // 빵을 구하고자 하는 사람 수; 1 <= m <= min (n^2, 30);

// 격자 : 갈 수 있는 곳은 0, 아닌 곳은 1로 변환
vector<vector<int>> grid(16, vector<int>(16, 0));
// 편의점 위치
unordered_map<int, pair<int, int>> stores;
// 사람 위치
unordered_map<int, pair<int, int>> men;
// 베이스캠프 위치(번호가 필요 없으므로 vector로)
vector<pair<int, int>> basecamps;

const vector<int> dr = { -1, 0, 0, 1 };
const vector<int> dc = { 0, -1, 1, 0 };

int _time = 0;

void Init()
{
    cin >> n >> m;

    grid.resize(n + 1, vector<int>(n + 1));

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> grid[i][j];
            if (grid[i][j] == 1) // 베이스캠프
            {
                basecamps.push_back({ i, j });
                grid[i][j] = 0;
            }
        }
    }

    for (int i = 1; i <= m; i++)
    {
        int x, y;
        cin >> x >> y;
        stores[i] = { x, y };
    }
}

void Conclude()
{
    cout << _time << endl;
}

void MoveAllMen()
{
    for (auto& man : men)
    {
        auto& mid = man.first;
        auto& mpos = man.second;

        if (mpos == stores[mid]) continue; // 이미 편의점에 도착했다면 이동 X

        vector<vector<int>> distance(n + 1, vector<int>(n + 1, -1));
        queue<pair<int, int>> q;
        vector<vector<pair<int, int>>> prev(n + 1, vector<pair<int, int>>(n + 1));

        q.push(mpos);
        distance[mpos.first][mpos.second] = 0;

        while (!q.empty())
        {
            pair<int, int> cur = q.front(); q.pop();
            auto& cr = cur.first;
            auto& cc = cur.second;

            if (cur == stores[mid]) break;

            for (int i = 0; i < 4; i++)
            {
                int nr = cr + dr[i];
                int nc = cc + dc[i];

                if (nr < 1 || nr > n || nc < 1 || nc > n) continue; // 범위를 벗어난 경우
                if (grid[nr][nc]) continue; // 이동 불가능한 지역
                if (distance[nr][nc] >= 0) continue; // 이미 방문

                distance[nr][nc] = distance[cr][cc] + 1;
                prev[nr][nc] = { cr, cc };
                q.push({ nr, nc });
            }
        }

        vector<pair<int, int>> path;
        for (pair<int, int> at = stores[mid]; at != make_pair(0, 0); at = prev[at.first][at.second])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        if (path.size() > 1)
        {
			mpos = path[1];
        }
    }
}

void BlockStore()
{
    for (auto& man : men)
    {
        auto& mid = man.first;
        auto& mpos = man.second;

        if (mpos == stores[mid])
        {
            grid[mpos.first][mpos.second] = 1;
        }
    }
}

bool IsAllArrived()
{
    for (auto& man : men)
    {
        auto& mid = man.first;
        auto& mpos = man.second;

        if (mpos != stores[mid]) return false;
    }
    return true;
}

void GoNearestBase(int num)
{
    pair<int, int>& wish = stores.at(num);
    auto& wr = wish.first;
    auto& wc = wish.second;

    // 편의점에서 각각의 basecamp까지의 거리를 조사
    vector<vector<int>> distance(n + 1, vector<int>(n + 1, -1));
    queue<pair<int, int>> q;

    distance[wr][wc] = 0;
    q.push(wish);

    while (!q.empty())
    {
        pair<int, int> cur = q.front(); q.pop();
        auto& cr = cur.first;
        auto& cc = cur.second;

        for (int i = 0; i < 4; i++)
        {
            int nr = cr + dr[i];
            int nc = cc + dc[i];

            if (nr < 1 || nr > n || nc < 1 || nc > n) continue; // 범위를 벗어난 경우
            if (grid[nr][nc] > 0) continue; // 이동 불가능한 지역
            if (distance[nr][nc] >= 0) continue; // 이미 방문

            distance[nr][nc] = distance[cr][cc] + 1;
            q.push({ nr, nc });
        }
    }

    // 들어갈 베이스캠프 결정
    men[num] = { 0, 0 };
    auto& mr = men[num].first;
    auto& mc = men[num].second;

    for (auto& basecamp : basecamps)
    {
        auto& br = basecamp.first;
        auto& bc = basecamp.second;

        if (distance[br][bc] == -1) continue; // 도달 불가능한 베이스캠프
        
        if (mr == 0 && mc == 0)
        {
			mr = br; mc = bc;
		}
        else
        {
            if (distance[mr][mc] > distance[br][bc])
            {
                mr = br; mc = bc;
            }
            else if (distance[mr][mc] == distance[br][bc])
            {
                if ((mr > br) || (mr == br && mc > bc))
                {
                    mr = br; mc = bc;
                }
            }
        }
    }
    // 베이스캠프에 들어갔으므로 해당 베이스캠프 위치는 이동이 불가능함
    grid[mr][mc] = 1;
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    while (true)
    {
        _time++;
        // 1. 본인이 가고 싶은 편의점 방향으로 최단거리로 이동
        MoveAllMen();

        // m초 이후 모두 편의점에 도착했다면 즉시 종료
        if (_time > m && IsAllArrived())
            break;

        // 2. 편의점에 도착한다면 해당 편의점은 더 이상 지나갈 수 없는 칸이 됨
        BlockStore();

        // 3. t번 사람은 자신이 가고 싶은 편의점과 가장 가까이 있는 베이스 캠프에 들어감
        if (_time <= m)
        {
            GoNearestBase(_time);
        }
    }

    Conclude();
    return 0;
}