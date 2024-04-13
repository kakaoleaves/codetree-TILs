#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>

using namespace std;

const int MAX_N = 10;
const int MAX_M = 10;

int n; // 보드의 세로 크기; // 3 <= n <= 10
int m; // 보드의 가로 크기; // 3 <= m <= 10

// 보드의 정보
vector<vector<char>> board(MAX_N, vector<char>(MAX_M));
bool visited[MAX_N + 1][MAX_M + 1][MAX_N + 1][MAX_M + 1];
int step[MAX_N + 1][MAX_M + 1][MAX_N + 1][MAX_M + 1];

// 빨간 구슬의 좌표
pair<int, int> red_pos;
// 파란 구슬의 좌표
pair<int, int> blue_pos;

// 최소의 턴 수
int ans = numeric_limits<int>::max();
const int MAX_T = 10;

// bfs를 위한 queue
queue<pair<pair<int, int>, pair<int, int>>> q;

// MAP 밖을 나왔다는 의미
const pair<int, int> OUT_OF_MAP = make_pair(MAX_N, MAX_M);

// 방향
enum { UP, DOWN, LEFT, RIGHT };

// 파란색이 맵에 있는가?
bool BlueExist()
{
	return blue_pos != OUT_OF_MAP;
}

bool RedExist()
{
	return red_pos != OUT_OF_MAP;
}


// dir 방향으로 기울였을 때 빨간색이 우선으로 움직여야 하는지 판단
bool RedMustFirst(int dir)
{
	int rx, ry, bx, by;
	tie(rx, ry) = red_pos;
	tie(bx, by) = blue_pos;

	if (dir == UP)
		return (ry == by && rx < bx);
	else if (dir == DOWN)
		return (ry == by && rx > bx);
	else if (dir == LEFT)
		return (rx == bx && ry < by);
	else
		return (rx == bx && ry > by);
}

// (x, y)로 진행이 가능한지 판단
bool CanGo(int x, int y)
{
	return board[x][y] != '#' && make_pair(x, y) != red_pos && make_pair(x, y) != blue_pos;
}

// (x,y)에서 dir 방향으로 이동 시 도착하는 위치 반환
pair<int, int> GetDestination(pair<int, int> pos, int dir)
{
	int dx[4] = { -1, 1, 0, 0 };
	int dy[4] = { 0, 0, -1 ,1 };

	int nx = pos.first + dx[dir];
	int ny = pos.second + dy[dir];

	if (!CanGo(nx, ny))
		return pos;

	if (board[nx][ny] == 'O')
		return OUT_OF_MAP;

	return GetDestination(make_pair(nx, ny), dir);
}

void Tilt(int dir)
{
	if (RedMustFirst(dir))
	{
		red_pos = GetDestination(red_pos, dir);
		blue_pos = GetDestination(blue_pos, dir);
	}
	else
	{
		blue_pos = GetDestination(blue_pos, dir);
		red_pos = GetDestination(red_pos, dir);
	}
}

void Push(pair<int, int> red_pos, pair<int, int> blue_pos, int new_step)
{
	q.push(make_pair(red_pos, blue_pos));
	visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = true;
	step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = new_step;
}

// BFS로 최소 이동 횟수를 구한다
void FindMin()
{
	while (!q.empty())
	{
		red_pos = q.front().first;
		blue_pos = q.front().second;

		int curr_step = step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second];
		q.pop();

		// 둘 중 하나의 사탕이 상자 안에 없다면 진행 X
		if (!BlueExist() || !RedExist())
			continue;

		for (int dir = 0; dir < 4; dir++)
		{
			// Tilt 전 위치 저장
			pair<int, int> tmp_red = red_pos;
			pair<int, int> tmp_blue = blue_pos;

			// 사탕의 위치 변경
			Tilt(dir);

			// 방문한 적이 없다면 새로 queue에 push
			if (!visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second])
				Push(red_pos, blue_pos, curr_step + 1);

			// Tilt 전 상태로 복원하여 그 다음 방향으로도 동일한 기회 제공
			red_pos = tmp_red;
			blue_pos = tmp_blue;
		}
	}

	for (int bx = 0; bx < n; bx++)
		for (int by = 0; by < n; by++)
			if (visited[OUT_OF_MAP.first][OUT_OF_MAP.second][bx][by])
				ans = min(ans, step[OUT_OF_MAP.first][OUT_OF_MAP.second][bx][by]);
}

void Init()
{
	cin >> n >> m;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> board[i][j];
			// 위치를 저장해두고, 맵에서 지워준다.
			if (board[i][j] == 'B')
			{
				blue_pos = { i, j };
				board[i][j] = '.';
			}
			else if (board[i][j] == 'R')
			{
				red_pos = { i, j };
				board[i][j] = '.';
			}
		}
	}
}

void Conclude()
{
	cout << ans << endl;
}


int main()
{
	Init();

	Push(red_pos, blue_pos, 0);
	FindMin();

	if (ans > MAX_T) ans = -1;

	Conclude();

	return 0;
}