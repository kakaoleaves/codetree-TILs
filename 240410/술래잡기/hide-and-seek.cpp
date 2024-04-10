#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>

using namespace std;

int n; // 격자의 크기; 5 <= n <= 99인 홀수
int m; // 도망자의 수; 1 <= m <= n^2
int h; // 나무의 수; 1 <= h <= n^2
int k; // 턴 수

// 도망자
struct Player
{
    int x; // 좌표
    int y; // 좌표
    int d; // 방향
};

vector<vector<int>> grid(100, vector<int>(100));

// 도망자
unordered_map<int, Player> runners;

// 술래
Player chaser;
int chaser_idx = -1;
vector<Player> chaser_path;

int ans = 0;

// 1 : 우 3 : 좌, 2 : 하 0 : 상
const vector<int> dr = { -1, 0, 1, 0 };
const vector<int> dc = { 0, 1, 0, -1 };
const int EYESIGHT = 3;

void PathInSprialOrder()
{
    // 안에서부터 시계방향
    int x = (n + 1) / 2;
    int y = (n + 1) / 2;

    int d = 0;
    int len = 1;
    int cnt = 0;

    while (len < n)
    {
        for (int i = 0; i < len; i++)
        {
            x += dr[d];
            y += dc[d];
            int tmpd = d;
            if (i == len - 1) tmpd = (d + 1) % 4;
            chaser_path.push_back({ x, y, tmpd });
        }

        cnt++;
        if (cnt == 2)
        {
            cnt = 0;
            len++;
        }

        d = (d + 1) % 4;
    }

    for (int i = 1; i < len; i++)
    {
        x += dr[d];
        y += dc[d];
        int tmpd = d;
        if (i == len - 1) tmpd = (d + 2) % 4;
        chaser_path.push_back({ x, y, tmpd });
    }

    // 밖에서부터 반시계방향
    len--;
    cnt = 0;
    d = (d + 2) % 4;

    for (int i = 0; i < len; i++)
    {
		x += dr[d];
		y += dc[d];
		int tmpd = d;
		if (i == len - 1) tmpd = (d - 1) % 4;
		chaser_path.push_back({ x, y, tmpd });
    }

    d = (d - 1 + 4) % 4;

    while (len > 0)
    {
        for (int i = 0; i < len; i++)
        {
            x += dr[d];
            y += dc[d];
            int tmpd = d;
            if (i == len - 1) tmpd = (d - 1 + 4) % 4;
            chaser_path.push_back({ x, y, tmpd });
        }

        cnt++;

        if (cnt == 2)
        {
			cnt = 0;
			len--;		
        }

        d = (d - 1 + 4) % 4;
    }

}

void Init()
{
    cin >> n >> m >> h >> k;

    grid.resize(n + 1, vector<int>(n + 1));

    // 도망자
    for (int i = 1; i <= m; i++)
    {
        int x, y, d;
        cin >> x >> y >> d;
        runners[i] = { x, y, d };
    }

    // 나무
    for (int i = 1; i <= h; i++)
    {
        int x, y;
        cin >> x >> y;
        grid[x][y] = 1;
    }

    chaser = { (n + 1) / 2, (n + 1) / 2, 0 };

    PathInSprialOrder();
}

void Conclude()
{
    cout << ans << endl;
}

void MoveRunners()
{
    for (auto& runner : runners)
    {
        auto& rid = runner.first;
        auto& rinfo = runner.second;

        // 술래와의 거리 계산
        int distance = abs(rinfo.x - chaser.x) + abs(rinfo.y - chaser.y);
        // 거리가 3보다 큰 도망자는 통과
        if (distance > 3) continue;

        // 거리가 3 이하인 도망자는 이동
        int nr = rinfo.x + dr[rinfo.d];
        int nc = rinfo.y + dc[rinfo.d];

        // 격자를 벗어나는 경우
        if (nr < 1 || nr > n || nc < 1 || nc > n)
        {
            // 방향을 반대로
            rinfo.d += 2;
            rinfo.d %= 4;

            nr = rinfo.x + dr[rinfo.d];
            nc = rinfo.y + dc[rinfo.d];

            // 해당 위치에 술래가 없다면 이동
            if (nr != chaser.x && nc != chaser.y)
            {
                rinfo.x = nr;
                rinfo.y = nr;
            }
        }
        else if (nr != chaser.x || nc != chaser.y)
        {
            rinfo.x = nr;
            rinfo.y = nc;
        }
    }
}

void MoveChaser()
{
    chaser_idx++;
    if (chaser_idx == chaser_path.size()) chaser_idx = 0;

    chaser.x = chaser_path[chaser_idx].x;
    chaser.y = chaser_path[chaser_idx].y;
    chaser.d = chaser_path[chaser_idx].d;
}

void CatchRunners(int turn)
{
    int chaser_x = chaser.x;
    int chaser_y = chaser.y;

    int max_chaser_x = chaser_x + dr[chaser.d] * EYESIGHT;
    int max_chaser_y = chaser_y + dc[chaser.d] * EYESIGHT;

    if (max_chaser_x < 1) max_chaser_x = 1;
    else if (max_chaser_x > n) max_chaser_x = n;

    if (max_chaser_y < 1) max_chaser_y = 1;
	else if (max_chaser_y > n) max_chaser_y = n;

    vector<int> caught_runners;
    for (auto& runner : runners)
    {
        auto& rid = runner.first;
        auto& rinfo = runner.second;

        if (rinfo.x >= chaser_x && rinfo.x <= max_chaser_x && rinfo.y >= chaser_y && rinfo.y <= max_chaser_y)
        {
            if (grid[rinfo.x][rinfo.y] == 0)
            {
				caught_runners.push_back(rid);
			}
		}
    }

    if (caught_runners.empty()) return;

    ans += caught_runners.size() * turn;

    for (auto& rid : caught_runners)
    {
		runners.erase(rid);
	}
}


int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (int t = 1; t <= k; t++)
    {
        MoveRunners();
        MoveChaser();
        CatchRunners(t);
    }

    Conclude();

    return 0;
}