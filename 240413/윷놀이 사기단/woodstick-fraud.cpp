#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Node
{
    int value; // 해당 칸의 점수
    vector<int> next; // 다음 지점의 번호 리스트
};

struct YutnoriBoard
{
    unordered_map<int, Node> board; // key: 번호, value: 노드

    YutnoriBoard()
    {
        for (int i = 0; i < 40; i += 2)
        {
            board[i].value = i;
            board[i].next.push_back(i + 2);
        }

        board[10].next.push_back(1013); // 10 -> 13
        board[1013].next.push_back(1016); // 13 -> 16
        board[1016].next.push_back(1019); // 16 -> 19
        board[1019].next.push_back(25); // 19 -> 25

        board[1013].value = 13;
        board[1016].value = 16;
        board[1019].value = 19;

        board[20].next.push_back(2022); // 20 -> 22
        board[2022].next.push_back(2024); // 22 -> 24
        board[2024].next.push_back(25); // 24 -> 25

        board[2022].value = 22;
        board[2024].value = 24;

        board[30].next.push_back(3028); // 30 -> 28
        board[3028].next.push_back(3027); // 28 -> 27
        board[3027].next.push_back(3026); // 27 -> 26
        board[3026].next.push_back(25); // 26 -> 25

        board[3028].value = 28;
        board[3027].value = 27;
        board[3026].value = 26;

        board[25].next.push_back(2530); // 25 -> 30
        board[2530].next.push_back(3535); // 30 -> 35
        board[3535].next.push_back(40); // 35 -> 40

        board[25].value = 25;
        board[2530].value = 30;
        board[3535].value = 35;

        board[40].value = 40; // 40 -> END
        board[40].next.push_back(-1); // END
        board[-1].value = 0;  // END
    }
};

vector<int> dice(10);
vector<int> piece(4, 0); // 말의 위치

YutnoriBoard yutnori;
int mx = 0;

void backtracking(int cnt, int sum)
{
    if (cnt == 10)
    {
        mx = max(mx, sum);
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (piece[i] == -1) continue; // 이미 도착한 말은 더 이상 움직일 수 없음

        int start = piece[i];   // 현재 위치
        int move = dice[cnt];   // 움직일 횟수
        int end = start;        // 움직인 위치

        while (move--)
        {
            auto& nxt = yutnori.board[end].next;

            if (nxt.empty()) break;

            if (nxt.size() == 1) end = nxt.front();
            else
            {
                if (start == 10 || start == 20 || start == 30) end = nxt.back();
                else end = nxt.front();
            }
        }

        if (end != -1 && find(piece.begin(), piece.end(), end) != piece.end()) continue; // 다른 말이 있는 경우

        int score = yutnori.board[end].value;

        piece[i] = end;
        backtracking(cnt + 1, sum + score);
        piece[i] = start;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 주사위 입력
    for (int i = 0; i < 10; i++)
    {
		cin >> dice[i];
	}
    
    // 윷놀이 판 생성

    backtracking(0, 0);

    cout << mx << "\n";

    return 0;
}