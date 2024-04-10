#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int n; // 4 <= n <= 10
int m; // 4 <= m <= 10
int k; // 1 <= k <= 1000

// 현재 턴
int turn = 1;

// 공격력 표시
vector<vector<int>> board(11, vector<int>(11));
// 공격한 턴 표시
vector<vector<int>> turns(11, vector<int>(11));
// 정비 시 공격과 무관한 포탑 확인용
vector<vector<bool>> effected(11, vector<bool>(11, false));

// 가장 약한 포탑 (공격자)
pair<int, int> weakest;
int& wr = weakest.first;
int& wc = weakest.second;

// 가장 강한 포탑 (피공격자)
pair<int, int> strongest;
int& sr = strongest.first;
int& sc = strongest.second;

void Init()
{
    cin >> n >> m >> k;

    board.resize(n + 1, vector<int>(m + 1));
    turns.resize(n + 1, vector<int>(m + 1));
    effected.resize(n + 1, vector<bool>(m + 1));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> board[i][j];
}

void Result()
{
    int ans = 0;
    for (int i = 1; i <= n; i++)
    {
        int mx = *max_element(board[i].begin(), board[i].end());
        ans = max(mx, ans);
    }
    cout << ans << '\n';
}

void DeclareAttacker()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (!board[i][j]) continue;

            // 비어있다면 첫 번째
            if (wr == 0 && wc == 0)
            {
                wr = i;
                wc = j;
            }
            else
            {
                if (board[wr][wc] > board[i][j])
                {
                    wr = i; wc = j;
                }
                else if (board[wr][wc] == board[i][j])
                {
                    if (turns[wr][wc] < turns[i][j])
                    {
                        wr = i; wc = j;
                    }
                    else if (turns[wr][wc] == turns[i][j])
                    {
                        if ((wr + wc < i + j) || (wr + wc == i + j && wc < j))
                        {
                            wr = i; wc = j;
                        }
                    }
                }
            }
        }
    }
}

void DeclareAttackee()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (!board[i][j]) continue;

            // 비어있다면 첫 번째
            if (sr == 0 && sc == 0)
            {
                sr = i;
                sc = j;
            }
            else
            {
                if (board[sr][sc] < board[i][j])
                {
                    sr = i; sc = j;
                }
                else if (board[sr][sc] == board[i][j])
                {
                    if (turns[sr][sc] > turns[i][j])
                    {
                        sr = i; sc = j;
                    }
                    else if (turns[sr][sc] == turns[i][j])
                    {
                        if ((sr + sc > i + j) || (sr + sc == i + j && sc > j))
                        {
                            sr = i; sc = j;
                        }
                    }
                }
            }
        }
    }
}

void AttackerPowerUp()
{
    // 공격자의 공격력 n + m만큼 증가
    board[wr][wc] += (n + m);
}

void AttackWithLaser(vector<pair<int,int>> path)
{
    for (auto& vertex : path)
    {
        if (vertex == weakest) continue;

        if (vertex == strongest)
        {
            board[sr][sc] -= board[wr][wc]; // 공격력만큼 피해
            if (board[sr][sc] < 0) board[sr][sc] = 0;
        }
        else
        {
            int& vr = vertex.first, &vc = vertex.second;
            board[vr][vc] -= (board[wr][wc] / 2);
            if (board[vr][vc] < 0) board[vr][vc] = 0;
        }
    }
}

void AttackWithShell(vector<pair<int,int>> adj)
{
    for (auto& vertex : adj)
    {
        if (vertex == strongest)
        {
            board[sr][sc] -= board[wr][wc]; // 공격력만큼 피해
            if (board[sr][sc] < 0) board[sr][sc] = 0;
        }
        else
        {
            int& vr = vertex.first, &vc = vertex.second;
            board[vr][vc] -= (board[wr][wc] / 2);
            if (board[vr][vc] < 0) board[vr][vc] = 0;
        }
    }
}

void Attack()
{
    // 최단 경로 찾기
    // 우 하 좌 상
    const vector<int> dr = { 0, 1, 0, -1 };
    const vector<int> dc = { 1, 0, -1, 0 };

    // 최단 경로 존재 여부 : 레이저 공격 가능 여부
    bool reachable = false;

    queue<pair<int, int>> q;
    // 모든 정점까지의 거리 -1로 초기화
    vector<vector<int>> distance(n + 1, vector<int>(m + 1, -1));
    // 이전 정점 추적
    vector<vector<pair<int, int>>> prev(n + 1, vector<pair<int, int>>(m + 1));

    distance[wr][wc] = 0;
    q.push(weakest);


    while (!q.empty())
    {
        pair<int, int> cur = q.front(); q.pop();
        int& cr = cur.first;
        int& cc = cur.second;

        // 목적지에 도달했으므로 중단
        if (cr == sr && cc == sc)
        {
            reachable = true;
            break;
        }

        // 인접 경로 탐색 (우선순위 반영)
        for (int i = 0; i < 4; i++)
        {
            int nr = cr + dr[i];
            int nc = cc + dc[i];

            // 반대쪽으로 나올 수 있도록
            if (nr < 1) nr = n;
            else if (nr > n) nr = 1;
            
            if (nc < 1) nc = m;
            else if (nc > m) nc = 1;

            // 공격력이 0인 포탑은 지나갈 수 없음
            if (!board[nr][nc]) continue;

            // 경로 확인 : -1이면 방문하지 않음
            if (distance[nr][nc] == -1)
            {
                distance[nr][nc] = distance[cr][cc] + 1;
                prev[nr][nc] = cur;
                q.push({nr, nc});
            }
        }
    }

    // 경로 역추적
    if (reachable)
    {
        vector<pair<int, int>> path;
        for (pair<int, int> at = strongest; at != make_pair(0, 0); at = prev[at.first][at.second])
        {
            path.push_back(at);
            effected[at.first][at.second] = true;
        }
        reverse(path.begin(), path.end());
        AttackWithLaser(path);
    }
    else
    {
        vector<pair<int, int>> adj;
        // 인접한 8개의 포탑과 자신 등록
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int r = sr + i;
                int c = sc + j;

                if (r < 1) r = n;
                else if (r > n) r = 1;
            
                if (c < 1) c = m;
                else if (c > m) c = 1;

                // 부서지지 않은 포탑만 등록
                if (board[r][c] > 0)
                {
                    adj.push_back({r, c});
                    effected[r][c] = true;
                }
            }
        }
        effected[wr][wc] = true; // 공격한 포탑도 기록

        AttackWithShell(adj);
    }

    // 공격했으니 공격한 턴 기록
    turns[wr][wc] = turn;
}

void RepairTurret()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (!effected[i][j] && board[i][j] > 0) board[i][j]++;
        }
    }
    
    // 정비 완료 후 effected 초기화
    effected.assign(n + 1, vector<bool>(m + 1, false));
}

void PrintTurret()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
}

bool CheckGamePossible()
{
    int cnt = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (board[i][j]) cnt++;
        }
    }

    return (cnt > 1);
}


int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    while (turn <= k)
    {
        turn++;
        // 공격자 결정
        DeclareAttacker();
        // 피공격자 결정
        DeclareAttackee();
        // 공격자의 공격력 증가
        AttackerPowerUp();
        // 공격
        Attack();
        // 포탑 정비
        RepairTurret();
        // PrintTurret();
        
        // 포탑이 1개가 된다면 그 즉시 중지
        if (!CheckGamePossible()) break;
    }

    Result();

    return 0;
}