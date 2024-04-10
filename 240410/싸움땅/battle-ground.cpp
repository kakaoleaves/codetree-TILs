#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

using namespace std;

int n; // 격자의 크기; 2 <= n <= 20
int m; // 플레이어의 수; 1 <= m <= min(n^2, 30)
int k; // 라운드 수; 1 <= k <= 500

struct Player
{
    int id;
    int x, y; // 플레이어의 위치; 1 <= x, y <= n
    int d; // 방향; 0 <= d <= 3
    int s; // 1 <= s <= 100; 초기 능력치
    int gun_s = 0; // 총의 능력치
    int score = 0; // 획득 점수
};

vector<vector<deque<int>>> grid(21, vector<deque<int>>(21));
vector<Player> players(31);

// 상 우 하 좌
const vector<int> dr = { -1, 0, 1, 0 };
const vector<int> dc = { 0, 1, 0, -1 };

void Init()
{
    cin >> n >> m >> k;

    grid.resize(n + 1, vector<deque<int>>(n + 1));
    players.resize(m + 1);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            int power;
            cin >> power;
            grid[i][j].push_back(power);
        }

    for (int i = 1; i <= m; i++)
    {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        players[i] = { i, x, y, d, s, 0, 0 };
    }

}

void Conclude()
{
    for (auto& player : players)
    {   
        if (player.id == 0) continue;
        cout << player.score << ' ';
    }
    cout << '\n';
}

void MovePlayer(int pid)
{
    auto& player = players.at(pid);

    int nr = player.x + dr[player.d];
    int nc = player.y + dc[player.d];

    // 범위를 벗어나는 경우 정반대 방향으로 방향을 바꾸어서 이동
    if (nr < 1 || nr > n || nc < 1 || nc > n)
    {
        player.d += 2;
        player.d %= 4;
        nr = player.x + dr[player.d];
        nc = player.y + dc[player.d];
    }

    player.x = nr;
    player.y = nc;
}

bool IsOtherThere(int pid)
{
    const auto& player = players.at(pid);

    for (const auto& other : players)
    {
        if (other.id == pid) continue;
        if (other.x == player.x && other.y == player.y) return true;
    }
    return false;
}

void CheckAndChangeGun(int pid)
{
    auto& player = players.at(pid);

    auto& pos_guns = grid[player.x][player.y];

    // 해당 위치에 총이 존재한다면
    if (!pos_guns.empty())
    {
        sort(pos_guns.begin(), pos_guns.end(), greater<int>());
        // 플레이어가 총이 없다면 총 하나를 줍는다.
        if (player.gun_s == 0)
        {
            player.gun_s = pos_guns.front();
            pos_guns.pop_front();
        }
        else
        {
            // 플레이어가 총이 있다면 총을 교체한다.
            int tmp = player.gun_s;
            player.gun_s = pos_guns.front();
            pos_guns.pop_front();
            pos_guns.push_back(tmp);
            sort(pos_guns.begin(), pos_guns.end(), greater<int>());
        }
    }
}

void Win(int pid, int point)
{
    auto& player = players.at(pid);
    player.score += point;

    CheckAndChangeGun(pid);
}

void Lose(int pid)
{
    auto& player = players.at(pid);

    // 총을 해당 위치에 내려놓는다.
    auto& pos_guns = grid[player.x][player.y];
    if (player.gun_s)
    {
        pos_guns.push_back(player.gun_s);
        sort(pos_guns.begin(), pos_guns.end(), greater<int>());
        player.gun_s = 0;
    }

    // 바라보는 방향으로 이동한다.
    for (int i = 0; i < 4; i++)
    {
        int nr = player.x + dr[player.d];
        int nc = player.y + dc[player.d];

        if (nr < 1 || nr > n || nc < 1 || nc > n)
        {
            player.d += 1;
            player.d %= 4;
            continue;
        }

        player.x = nr;
        player.y = nc;
        if (IsOtherThere(pid))
        {
            player.x -= dr[player.d];
            player.y -= dr[player.d];
            player.d += 1;
            player.d %= 4;
            continue;
        }
        break;
    }

    // 빈 칸으로 이동했으면 총을 줍는다.
    CheckAndChangeGun(pid);
}

void Fight(int pid)
{
    const auto& player = players.at(pid);
    int winner;
    int loser;    
    int point;

    for (const auto& other : players)
    {
        if (other.id == pid) continue;
        if (other.x == player.x && other.y == player.y)
        {
            int other_total_s = other.s + other.gun_s;
            int player_total_s = player.s + player.gun_s;
            point = abs(other_total_s - player_total_s);

            if (other_total_s > player_total_s)
            {
                winner = other.id, loser = pid;
            }
            else if (other_total_s == player_total_s)
            {
                if (other.s > player.s)
                    winner = other.id, loser = pid;
                else
                    winner = pid, loser = other.id;
            }
            else
            {
                winner = pid, loser = other.id;
            }
            break;
        }
    }

    Win(winner, point);
    Lose(loser);
}

void MoveAllPlayer()
{
    // 플레이어를 순차적으로 이동시킨다.
    for (auto& player : players)
    {
        if (player.id == 0) continue;
        // 1-1.
        MovePlayer(player.id);

        if (IsOtherThere(player.id))
        {
            Fight(player.id);
        }
        else
        {
            // cout << "OTHER NO!" << endl;
            CheckAndChangeGun(player.id);
        }
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (int i = 1; i <= k; i++)
    {
        // 1-1. 플레이어 한 칸 이동
        MoveAllPlayer();
    }

    Conclude();

    return 0;
}