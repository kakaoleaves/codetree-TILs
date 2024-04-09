#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_map>
#include <limits>
#include <cmath>

using namespace std;

int N; // 미로의 크기; 4 <= N <= 10
int M; // 참가자 수; 1 <= M <= 10
int K; // 게임 시간; 1 <= K <= 100

vector<vector<int>> maze(11, vector<int>(11)); // 미로 상태
unordered_map<int, pair<int, int>> participants; // 참여자들 : key = id, value = 좌표
pair<int, int> exit_pos; // 출구
auto& [er, ec] = exit_pos;

int ans = 0; // 참여자들의 이동거리 합

const vector<int> dr = { -1, 1, 0, 0 };
const vector<int> dc = { 0, 0, 1, -1 };

void init()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> K;

    maze.resize(N + 1, vector<int>(N + 1));

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cin >> maze[i][j];
        }
    }

    for (int i = 1; i <= M; i++)
    {
        int _r, _c;
        cin >> _r >> _c;
        participants[i] = { _r, _c };
    }

    auto& [mr, mc] = exit_pos;
    cin >> mr >> mc;
}

void output()
{
    cout << ans << '\n' << er << ' ' << ec << '\n';
}

void move_participants()
{
    // 출구에 도착한 참가자들을 저장할 벡터
    vector<int> participants_to_remove;

    // 참가자들 이동
    for (auto& [pid, pos] : participants)
    {
        auto& [r, c] = pos;

        int d_from_exit = abs(r - er) + abs(c - ec); // 출구까지의 거리

        for (int i = 0; i < 4; i++)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr < 1 || nr > N || nc < 1 || nc > N) continue; // 범위를 벗어나는 경우
            if (maze[nr][nc] > 0) continue; // 벽이 있는 경우

            int d = abs(nr - er) + abs(nc - ec);

            if (d < d_from_exit) // 이동했을 때 출구까지의 거리가 더 짧은 경우 갱신
            {
                r = nr;
                c = nc;
                ans += 1;

                // 이 때 출구에 도착했다면 참가자 제거
                if (nr == er && nc == ec)
                {
                    participants_to_remove.push_back(pid);
                }
                break;
            }
        }

    }

    // 출구에 도착한 참가자들 제거
    for (int pid : participants_to_remove)
    {
		participants.erase(pid);
	}
}

void decideSquare(pair<int, int>& start, int& len)
{
    // 참여자들의 좌표 표시
    vector<vector<int>> maze_participants(N + 1, vector<int>(N + 1));

    for (auto& [pid, pos] : participants)
    {
		auto& [r, c] = pos;
		maze_participants[r][c]++;
	}

    const vector<int> dx = { -1, -1, 1, 1 }; // 상 -> 하
    const vector<int> dy = { -1, 1, 1, -1 }; // 좌 -> 우

    while (len <= N)
    {
        for (int i = 1; i < N - len; i++)
        {
            for (int j = 1; j < N - len; j++)
            {
                if (er < i || er >= i + len || ec < j || ec >= j + len) continue; // 출구가 정사각형 밖에 있는 경우

                for (int x = i; x < i + len; x++)
                {
                    for (int y = j; y < j + len; y++)
                    {
                        if (maze_participants[x][y] > 0)
                        {
                            start = { i, j };
                            return;
                        }
					}
				}
			}
        }

        len++;
    }
}

void rotateSquare(pair<int, int>& start, int& len)
{
    auto copy_maze = maze;
    auto& [sr, sc] = start;

    for (int r = sr; r < sr + len; r++)
    {
        for (int c = sc; c < sc + len; c++)
        {
            int nr = c - sc + sr;
            int nc = len - 1 - (r - sr) + sc;
            maze[nr][nc] = copy_maze[r][c];
            if (maze[nr][nc] > 0) maze[nr][nc]--; // 벽 내구도 감소
		}
	}

    // 참가자들의 좌표도 회전
    for (auto& [pid, pos] : participants)
    {
        auto& [r, c] = pos;
        if (r < sr || r >= sr + len || c < sc || c >= sc + len) continue; // 정사각형 밖에 있는 경우
        int nr = c - sc + sr;
        int nc = len - 1 - (r - sr) + sc;

        r = nr;
        c = nc;
    }

    // 출구도 회전
    if (er >= sr && er < sr + len && ec >= sc && ec < sc + len)
    {
		int nr = ec - sc + sr;
		int nc = len - 1 - (er - sr) + sc;
		er = nr;
		ec = nc;
	}
}

int main()
{
    init();

    while (K--)
    {
        if (participants.empty()) // 모든 참가자가 이미 탈출했다면
        {
			break;
		}

        move_participants();

        // 정사각형 찾기
        pair<int, int> upper_left; // 정사각형의 왼쪽 상단 좌표
        int length = 2; // 정사각형의 한 변의 길이
        decideSquare(upper_left, length);

        // 정사각형 회전
        rotateSquare(upper_left, length);
    }

    output();

    return 0;
}