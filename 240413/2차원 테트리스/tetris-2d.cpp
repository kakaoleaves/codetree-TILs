#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using namespace std;

struct BlockInfo
{
    int t, x, y;
};

int n; // 블록을 놓은 횟수; 1 <= n <= 10000
vector<BlockInfo> blocks(10000);

vector<vector<int>> green(6, vector<int>(4, 0));
vector<vector<int>> blue(6, vector<int>(4, 0)); // [i, j] -> [j, i]로 변환

int score; // 점수
int cnt; // 블록의 개수

void BlockDown(int t, int x, int y, vector<vector<int>>& board, bool isGreen)
{
    int nt, nx, ny;

    nx = 0;
    ny = isGreen ? y : x;

    if (!isGreen && t == 2)
    {
        nt = 3;
    }
    else if (!isGreen && t == 3)
    {
        nt = 2;
    }
    else
    {
        nt = t;
    }

    switch (nt)
    {
    case 1:
        // 1x1
        while (nx < 5 && board[nx + 1][ny] == 0)
        {
			nx++;
		}
        board[nx][ny] = 1;
        break;
    case 2:
        // 1x2
        while (nx < 5 && board[nx + 1][ny] == 0 && board[nx + 1][ny + 1] == 0)
        {
            nx++;
        }
        board[nx][ny] = 1;
        board[nx][ny + 1] = 1;
        break;
    case 3:
        // 2x1
        while (nx < 5 && board[nx + 1][ny] == 0)
        {
			nx++;
		}
        board[nx - 1][ny] = 1;
        board[nx][ny] = 1;
        break;
    default:
        break;
    }

}

void Check(vector<vector<int>>& board)
{
    vector<vector<int>> newBoard(6, vector<int>(4, 0));

    for (int i = 0; i < 2; i++)
    {
        newBoard[i] = board[i];
    }

    int idx = 2;
    for (int i = 2; i < 6; i++)
    {
        if (all_of(board[i].begin(), board[i].end(), [](int x) { return x == 1; }))
        {
			score++;
		}
        else
        {
            newBoard[idx] = board[i];
			idx++;
		}
	}

    stable_partition(newBoard.begin(), newBoard.end(), [](const vector<int>& v) { return all_of(v.begin(), v.end(), [](int x) { return x == 0; }); });

    int row = 0;
    for (int i = 0; i < 2; i++)
    {
        if (any_of(newBoard[i].begin(), newBoard[i].end(), [](int x) { return x == 1; }))
        {
            row++;
        }
    }

    for (int i = 0; i < row; i++)
    {
        newBoard[5 - i] = vector<int>(4, 0);
    }

    stable_partition(newBoard.begin(), newBoard.end(), [](const vector<int>& v) { return all_of(v.begin(), v.end(), [](int x) { return x == 0; }); });

    board = newBoard;
}

void PrintGreen()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
			cout << green[i][j] << ' ';
		}
		cout << '\n';
    }
}

void PrintBlue()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
		{
            cout << blue[j][i] << ' ';
        }
        cout << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n; i++) {
		cin >> blocks[i].t >> blocks[i].x >> blocks[i].y;
	}

    for (int i = 0; i < n; i++)
    {
        const auto& [t, x, y] = blocks[i];

        // 블록을 놓는다.
        BlockDown(t, x, y, green, true);
        BlockDown(t, x, y, blue, false);

        //cout << "[turn " << i + 1 << "]" << endl;
        //cout << "Before Check" << endl;
        //PrintGreen();
        //cout << endl;
        //PrintBlue();
        //cout << "-------------------------------------------" << endl;


        // 블록이 꽉 찼는지 확인한다.
        Check(green);
        Check(blue);

        //cout << "After Check" << endl;
        //PrintGreen();
        //cout << endl;
        //PrintBlue();
        //cout << "==========================================" << endl;
    }

    // 블록 개수 계산
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (green[i][j] == 1) cnt++;
            if (blue[i][j] == 1) cnt++;
		}
	}

    cout << score << '\n' << cnt << '\n';

    return 0;
}