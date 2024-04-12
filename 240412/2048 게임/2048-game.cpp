#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n; // 보드의 크기; 1<= n <= 20
vector<vector<int>> board(20, vector<int>(20, 0));
int ans = 0;

enum { UP, DOWN, LEFT, RIGHT };

void Print()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << board[i][j] << ' ';
        cout << endl;
    }
}

void Move(int dir)
{
	for (int i = 0; i < n; i++)
	{
		vector<int> tmp;

		bool isRow = (dir == LEFT || dir == RIGHT); // 좌우 : 행, 상하 : 열
		// 각 열이나 행을 복사해서 저장함.
		for (int j = 0; j < n; j++)
		{
			tmp.push_back(isRow ? board[i][j] : board[j][i]);
		}

		// 각 방향에 맞게 0이 없도록 원하는 방향으로 이동시킴
		bool isZeroUp = (dir == RIGHT || dir == DOWN);
		stable_partition(tmp.begin(), tmp.end(), [isZeroUp](int x) { return (isZeroUp ? x == 0 : x != 0); });

		// 0이 아닌 인접한 값에 대한 합침 작업 수행
		if (isZeroUp)
		{
			for (int j = n - 1; j > 0; j--)
			{
				if (tmp[j] == tmp[j - 1] && tmp[j] != 0)
				{
					tmp[j] *= 2;
					tmp[j - 1] = 0;
					j--;
				}
				else if (tmp[j] == 0) break;
			}
		}
		else
		{
			for (int j = 0; j < n - 1; j++)
			{
				if (tmp[j] == tmp[j + 1] && tmp[j] != 0)
				{
					tmp[j] = 0;
					tmp[j + 1] *= 2;
					j++;
				}
				else if (tmp[j] == 0) break;
			}
		}

		// 합침 작업 수행 후 다시 0이 없도록 방향 이동
		stable_partition(tmp.begin(), tmp.end(), [isZeroUp](int x) { return (isZeroUp ? x == 0 : x != 0); });

		// 본래 열이나 행에 복사
		for (int j = 0; j < n; j++)
		{
			if (isRow) board[i][j] = tmp[j];
			else board[j][i] = tmp[j];
		}
	}
}


void backtracking(int cnt)
{
    if (cnt == 5)
    {
        for (int i = 0; i < n; i++)
        {
            int mx = *max_element(board[i].begin(), board[i].end());
            ans = max(ans, mx);
        }
        //cout << "================ 완료 ================" << endl;
        return;
    }

    vector<vector<int>> tmp = board;

    for (int i = 0; i < 4; i++)
    {
        // 원하는 방향으로 전체 블록을 옮긴다
        Move(i);
        backtracking(cnt + 1);
        board = tmp;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> board[i][j];
        }
    }

    backtracking(0);

    cout << ans << '\n';

    return 0;
}