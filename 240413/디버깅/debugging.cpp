#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n; // 세로선의 개수; 2 <= n <= 10
int m; // 가로선의 개수; 0 <= m <= (n-1)* h
int h; // 가로선을 놓을 수 있는 위치의 개수; 1 <= h <= 30

vector<vector<bool>> ladder(31, vector<bool>(11, false));
// ladder[a][b] = true; a번 세로선과 a + 1번 세로선을 b번 점선 위치에서 연결

// b번 세로선과 b + 1번 세로선을 a번 점선 위치에서 연결
int a; // 1 <= a <= h
int b; // 1 <= b <= n - 1


bool check()
{
    for (int c = 1; c <= n; c++)
    {
        int pos = c;
        for (int r = 1; r <= h; r++)
        {
            if (ladder[r][pos]) pos++;
			else if (ladder[r][pos - 1]) pos--;
        }
        if (pos != c) return false;
    }
    return true;
}

int solve(int count = 0, int mc = 3, int x = 1, int y = 1)
{
    if (check()) return count;
    if (count >= mc) return -1;

    int ret = -1;

    for (int r = x; r <= h; r++)
    {
        for (int c = y; c < n; c++)
        {
			if (ladder[r][c] || ladder[r][c - 1] || ladder[r][c + 1]) continue;

			ladder[r][c] = true;
			int temp = solve(count + 1, mc, r, c + 2);
            if (temp != -1)
            {
                if (ret == -1 || ret > temp) ret = temp;
                mc = ret;
            }
			ladder[r][c] = false;
		}
        y = 1;
    }

    return ret;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> h;

    for (int i = 0; i < m; i++)
    {
		cin >> a >> b;
		ladder[a][b] = true;
	}

    cout << solve() << '\n';

    return 0;
}