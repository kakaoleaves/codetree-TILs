#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m; // 4 <= n, m <= 500
vector<vector<int>> paper;
vector<vector<bool>> visited;
int ans;

vector<int> dx = { 0, 0, 1, -1 };
vector<int> dy = { 1, -1, 0, 0 };

void Tetromino(int x, int y, int sum, int cnt) {
	if (cnt == 4) {
		ans = max(ans, sum);
		return;
	}

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
		if (visited[nx][ny]) continue;

		visited[nx][ny] = true;
		Tetromino(nx, ny, sum + paper[nx][ny], cnt + 1);
		visited[nx][ny] = false;
	}	
}

void T_Teromino(int x, int y) {
	// [x, y]가 중심일 때 ㅗ 모양을 만들 수 있는 경우는 4가지
	for (int i = 0; i < 4; i++) {
		int sum = paper[x][y];
		bool flag = true;
		for (int j = 0; j < 3; j++) {
			int nx = x + dx[(i + j) % 4];
			int ny = y + dy[(i + j) % 4];

			if (nx < 0 || nx >= n || ny < 0 || ny >= m) {
				flag = false;
				break;
			}
			sum += paper[nx][ny];
		}
		if (flag) ans = max(ans, sum);
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	paper.resize(n, vector<int>(m, 0));
	visited.resize(n, vector<bool>(m, false));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> paper[i][j];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			visited[i][j] = true;
			Tetromino(i, j, paper[i][j], 1);
			T_Teromino(i, j);
			visited[i][j] = false;
		}

	cout << ans << '\n';

	return 0;
}