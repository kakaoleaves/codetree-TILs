#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int L; // 체스판의 크기; 3 <= L <= 40
int N; // 기사의 수; 1 <= N <= 30
int Q; // 명령의 수; 1 <= Q <= 100

enum { BLANK, TRAP, WALL };

struct Knight {
    int r;
    int c;
    int h;
    int w;
    int k;
};

int total_dmg = 0;

vector<vector<int>> board(41, vector<int>(41));
map<int, Knight> knights;
vector<pair<int, int>> commands(100);

enum { UP, RIGHT, DOWN, LEFT };

const vector<int> dr = { -1, 0, 1, 0 };
const vector<int> dc = { 0, 1, 0, -1 };

vector<vector<int>> kn_board(41, vector<int>(41));


void Init()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> L >> N >> Q;

    board.resize(L + 1, vector<int>(L + 1));
    kn_board.resize(L + 1, vector<int>(L + 1));
    commands.resize(Q);

    // 체스판 정보
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            cin >> board[i][j];
        }
    }

    // 기사
    for (int i = 1; i <= N; i++)
    {
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        Knight kn = {r, c, h, w, k};
        knights[i] = kn;
    }

    for (int i = 0; i < Q; i++)
    {
        int _i, _d;
        cin >> _i >> _d;
        commands[i] = { _i, _d };
    }

    // cout << "Init End" << endl;
}

bool IsValid(int r, int c)
{
    return (1 <= r && r <= L && 1 <= c && c <= L);
}

void MoveKnight(int i, int d)
{
    if (knights.find(i) == knights.end()) return;

    auto& knight = knights.at(i);
    auto& [r, c, h, w, k] = knight;
    int trap_cnt = 0;

    r += dr[d];
    c += dc[d];

    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            if (board[i][j] == TRAP) trap_cnt++;            
        }
    }

    k -= trap_cnt;
    total_dmg += trap_cnt;
}

bool IsMovePossible(int i, int d)
{
    if (knights.find(i) == knights.end()) return false;

    const auto& [r, c, h, w, k] = knights[i];

    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            int nr = r + dr[d];
            int nc = r + dc[d];

            if (!IsValid(nr, nc)) return false; // 범위 초과
            if (board[nr][nc] == WALL) return false; // 벽
            if (kn_board[nr][nc] != 0 && kn_board[nr][nc] != i) // 자신과 다른 기사
            {
                if (IsMovePossible(kn_board[nr][nc], d) == false) return false;
            }
        }
    }

    MoveKnight(i, d);

    return true;
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (auto& cmd : commands)
    {
        const auto& [i, d] = cmd;

        if (knights.find(i) == knights.end()) continue; // 사라진 기사는 통과

        // 기사들에 대한 체스판 정보 생성
        kn_board.assign(L + 1, vector<int>(L + 1));
        for (auto& knight: knights)
        {
            const auto& [kn_id, kn_value] = knight;
            const auto& [r, c, h, w, k] = kn_value;

            for (int i = r; i < r + h; i++)
            {
                for (int j = c; j < c + w; j++)
                {
                    kn_board[i][j] = kn_id;                    
                }
            }
        }

        if (!IsMovePossible(i, d)) continue;

        for (auto it = knights.begin(); it != knights.end();)
        {
            auto& [kn_id, kn] = *it;

            if (kn.k < 0) {
                it = knights.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    cout << total_dmg << '\n';

    return 0;
}