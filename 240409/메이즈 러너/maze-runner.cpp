#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int N; // 미로의 크기; 4 <= N <= 10
int M; // 참가자 수; 1 <= M <= 10
int K; // 게임 시간; 1 <= K <= 100

// 미로 벽들의 상태
vector<vector<int>> maze(11, vector<int>(11));

// 참가자
unordered_map<int, pair<int, int>> participants;

// 출구의 위치 정보
pair<int, int> exit_pos;
int& er = exit_pos.first;
int& ec = exit_pos.second;

// 정답(모든 참가자들의 이동 거리 합)
int ans = 0;

// 회전해야 하는 최소 정사각형
pair<int, int> ul;
int& sr = ul.first;
int& sc = ul.second;
int square_size;

void Init()
{
    cin >> N >> M >> K;

    maze.resize(N + 1, vector<int>(N + 1));

    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            cin >> maze[i][j];

    for (int i = 1; i <= M; i++)
    {
        int _r, _c;
        cin >> _r >> _c;
        participants[i] = { _r, _c };
    }

    cin >> er >> ec;
}

void Result()
{
    cout << ans << '\n' << er << ' ' << ec << '\n';
}

void MoveAllParticipants()
{
    vector<int> arrived;

    for (auto& participant : participants)
    {
        auto& id = participant.first;
        auto& pos = participant.second;

        auto& r = pos.first;
        auto& c = pos.second;

        // 출구에 있는 경우 스킵
        if (er == r && ec == c) continue;

        // 행이 다르면 행을 이동시킨다.
        if (r != er)
        {
            int nr = r, nc = c;
            if (nr > er) nr--;
            else nr++;

            // 벽이 없다면 행을 이동한다.
            // 이동했으니 바로 다음 참가자로
            if (!maze[nr][nc])
            {
                r = nr;
                c = nc;
                ans++;
                if (r == er && c == ec) // 출구에 도착하면 벡터에 추가
                    arrived.push_back(id);
                continue;
            }
        }

        // 열이 다른 경우 열을 이동시킨다.
        if (c != ec)
        {
            int nr = r, nc = c;
            if (nc > ec) nc--;
            else nc++;

            if (!maze[nr][nc])
            {
                r = nr;
                c = nc;
                ans++;
                if (r == er && c == ec) // 출구에 도착하면 벡터에 추가
                    arrived.push_back(id);
                continue;
            }
        }
    }

    if (!arrived.empty())
    {
        for (auto& id : arrived)
        {
            participants.erase(id);
        }
    }
}

void FindMinimumSquare()
{
    vector<vector<int>> tmp(N + 1, vector<int>(N + 1));

    for (auto& participant : participants)
    {
        auto& id = participant.first;
        auto& pos = participant.second;

        auto& r = pos.first;
        auto& c = pos.second;

        tmp[r][c]++;
    }

    // 가장 작은 정사각형부터 모든 정사각형에서
    for (int sz = 2; sz <= N; sz++)
    {
        // 좌상단 r좌표가 작은 것부터
        for (int r1 = 1; r1 <= N - sz + 1; r1++)
        {
            for (int c1 = 1; c1 <= N - sz + 1; c1++)
            {
                int r2 = r1 + sz - 1;
                int c2 = c1 + sz - 1;

                // 출구가 범위 밖에 있다면 스킵
                if (er < r1 || er > r2 || ec < c1 || ec > c2) continue;

                // 한 명 이상의 참가자가 존재하는지 확인
                for (int x = r1; x <= r2; x++)
                {
                    for (int y = c1; y <= c2; y++)
                    {
                        if (tmp[x][y] > 0)
                        {
                            sr = r1;
                            sc = c1;
                            square_size = sz;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void RotateSquare()
{
    // 벽들을 1 감소
    for (int r = sr; r < sr + square_size; r++)
    {
        for (int c = sc; c < sc + square_size; c++)
        {
            if (maze[r][c]) maze[r][c]--;
        }
    }

    auto copy_maze = maze;

    // 시계방향으로 90' 회전
    for (int r = sr; r < sr + square_size; r++)
    {
        for (int c = sc; c < sc + square_size; c++)
        {
            // (sr, sc)를 (0, 0)으로 이동
            int _or = r - sr, _oc = c - sc;
            // 회전 좌표는 (i, j) -> (j, n - 1 - i)
            int _rr = _oc, _rc = square_size - 1 - _or;
            // 복원
            maze[_rr + sr][_rc + sc] = copy_maze[r][c];
        }
    }
}

void RotateParticipants()
{
    int dr = sr + square_size - 1;
    int dc = sc + square_size - 1;

    // 참가자 회전
    for (auto& participant : participants)
    {
        auto& id = participant.first;
        auto& pos = participant.second;

        auto& r = pos.first;
        auto& c = pos.second;

        if (r >= sr && r <= dr && c >= sc && c <= dc)
        {
            int _or = r - sr, _oc = c - sc;
            int _rr = _oc, _rc = square_size - 1 - _or;

            r = _rr + sr;
            c = _rc + sc;
        }
    }
}

void RotateExit()
{
    int dr = sr + square_size - 1;
    int dc = sc + square_size - 1;

    // 출구 회전
    if (er >= sr && er <= dr && ec >= sc && ec <= dc)
    {
        int _or = er - sr, _oc = ec - sc;
        int _rr = _oc, _rc = square_size - 1 - _or;

        er = _rr + sr;
        ec = _rc + sc;
    }
}

void Print()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
            cout << maze[i][j] << ' ';
        cout << '\n';
    }
}

void PrintParticipants()
{
    vector<vector<int>> tmp(N + 1, vector<int>(N + 1));

    for (auto& participant : participants)
    {
        auto& id = participant.first;
        auto& pos = participant.second;

        auto& r = pos.first;
        auto& c = pos.second;

        tmp[r][c] = id;
    }

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
            cout << tmp[i][j] << ' ';
        cout << '\n';
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    while (K--)
    {
        // 모든 참가자를 이동시킨다.
        MoveAllParticipants();

        // 이동시킨 뒤, 미로 안에 참가자가 없다면 종료한다.
        if (participants.empty()) break;

        // 한 명 이상의 참가자와 출구를 포함한 가장 작은 정사각형을 찾는다.
        FindMinimumSquare();

        // 정사각형을 회전한다.
        RotateSquare();
        RotateParticipants();
        RotateExit();
    }

    Result();

    return 0;
}