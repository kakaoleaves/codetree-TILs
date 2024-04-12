#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <deque>
#include <unordered_map>

using namespace std;

int m; // 몬스터의 마리 수
int t; // 진행되는 턴의 수
pair<int, int> pacman;

struct Monster
{
    int r;
    int c;
    int d;
};

int key = 1;
unordered_map<int, Monster> monsters;
vector<Monster> eggs;

// 몬스터 위치 기록
vector<vector<deque<int>>> monster_grid(5, vector<deque<int>>(5));

// 시체의 남은 턴 수 기록
vector<vector<int>> body(5, vector<int>(5));

//                                  ↑, ↖, ←, ↙, ↓, ↘, →, ↗ 
const vector<int> monster_dr = { 0, -1, -1, 0, 1, 1, 1, 0, -1 };
const vector<int> monster_dc = { 0, 0, -1, -1, -1, 0, 1, 1, 1 };

//                              상 좌 하 우
const vector<int> pacman_dr = { -1, 0, 1, 0 };
const vector<int> pacman_dc = { 0, -1, 0, 1 };
enum {UP, LEFT, DOWN, RIGHT};

void Init()
{
    cin >> m >> t;
    cin >> pacman.first >> pacman.second;

    for (int i = 0; i < m; i++)
    {
        int r, c, d;
        cin >> r >> c >> d;
        monsters[key] = { r, c, d };
        monster_grid[r][c].push_back(key);
        key++;
    }
}

void Conclude()
{
    cout << monsters.size() << endl;
}

void TryMonsterCopy()
{
    // 자신 위치와 방향을 가진 알을 생성
    for (auto& _monster : monsters)
    {
        eggs.push_back(_monster.second);
    }
}

void MoveMonster()
{
    // 자신이 가진 방향대로 한 칸 이동
    for (auto& _monster : monsters)
    {
        auto& m_id = _monster.first;
        auto& monster = _monster.second;
        for (int i = 0; i < 8; i++)
        {
            int nr = monster.r + monster_dr[monster.d];
            int nc = monster.c + monster_dc[monster.d];

            // 몬스터 시체 or 팩맨 or 범위 초과 시 반시계 방향으로 45도 회전 (모두 돌았으면 움직이지 않음)
            // 범위 초과
            bool isRangeInValid = nr < 1 || nr > 4 || nc < 1 || nc > 4;
            // 팩맨 위치
            bool isPacmanPos = nr == pacman.first && nc == pacman.second;
            // 시체 위치
            bool isBodyPos = body[nr][nc] > 0;

            if (isPacmanPos || isRangeInValid || isBodyPos)
            {
                monster.d++;
                if (monster.d > 8) monster.d = 1;
                continue;
            }

            monster_grid[monster.r][monster.c].pop_front();
            monster.r = nr;
            monster.c = nc;
            monster_grid[nr][nc].push_back(m_id);

            break;
        }
    }
}

void backtracking(int mcnt, pair<int,int> pm, string path, vector<vector<bool>>& visited, int sum, int& mx, string& mpath)
{
    if (mcnt == 3)
    {
        if (sum > mx)
        {
            mx = sum;
            mpath = path;
        }
        else if (sum == mx)
        {
            if (mpath == "") mpath = path;
            mpath = min(path, mpath);
        }
        return;
    }


    for (int i = 0; i < 4; i++)
    {
        auto tmp = visited;

        int nr = pm.first + pacman_dr[i];
        int nc = pm.second + pacman_dc[i];

        if (nr < 1 || nr > 4 || nc < 1 || nc > 4) continue;

        int next_sum = sum;
        if (!visited[nr][nc]) next_sum += monster_grid[nr][nc].size();
        visited[nr][nc] = true;

        backtracking(mcnt + 1, {nr, nc}, path + to_string(i), visited, next_sum, mx, mpath);

        visited = tmp;
    }

}

void MovePacman()
{
    // 3칸 이동 (몬스터를 가장 많이 먹을 수 있는 방향)
    // 상좌하우 우선순위

    // 먹을 수 있는 경로
    string path = "";
    // 먹을 수 있는 몬스터 수
    int mx = 0;
    // 방문 여부
    vector<vector<bool>> visited(5, vector<bool>(5, false));

    backtracking(0, pacman, path, visited, 0, mx, path);

    for (auto& c : path)
    {
        int idx = c - '0';
        pacman.first += pacman_dr[idx];
        pacman.second += pacman_dc[idx];

        if (!monster_grid[pacman.first][pacman.second].empty())
        {
            for (const auto& id : monster_grid[pacman.first][pacman.second])
            {
                monsters.erase(id);
            }

            body[pacman.first][pacman.second] = 2;
            monster_grid[pacman.first][pacman.second].clear();
        }
    }
}

void DestroyMonsterBody()
{
    // 2턴 유지된 시체 제거
    for (int i = 1; i <= 4; i++)
        for (int j = 1; j <= 4; j++)
            if (body[i][j] > 0) body[i][j]--;
}

void CompleteMonsterCopy()
{
    // 알이었던 몬스터 부화
    for (const auto& egg: eggs)
    {
        monster_grid[egg.r][egg.c].push_back(key);
        monsters[key] = egg;
        key++;
    }
    eggs.clear();
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (int i = 1; i <= t; i++)
    {
        TryMonsterCopy();
        MoveMonster();
        DestroyMonsterBody();
        MovePacman();
        CompleteMonsterCopy();
    }

    Conclude();

    return 0;
}