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
    int dmg = 0;
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
        Knight kn = { r, c, h, w, k, 0 };
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

void MoveKnight(int id, int direction)
{
    if (knights.find(id) == knights.end()) return;

    auto& knight = knights.at(id);
    auto& [r, c, h, w, k, dmg] = knight;

    r += dr[direction];
    c += dc[direction];
}

void UpdateKnightDmg(int id)
{
    if (knights.find(id) == knights.end()) return;
    auto& knight = knights.at(id);
    auto& [r, c, h, w, k, dmg] = knight;

    int cur_dmg = 0;
    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            if (board[i][j] == TRAP) cur_dmg++;
        }
    }

    if (cur_dmg >= k) knights.erase(id);
    else
    {
        k -= cur_dmg;
        dmg += cur_dmg;
    }
}

bool IsMovePossible(int id, int direction, vector<bool>& visited, bool commanded = true)
{
    if (knights.find(id) == knights.end()) return false;

    const auto& [r, c, h, w, k, dmg] = knights[id];

    // 벽과 범위에 대한 여부 확인
    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            int nr = i + dr[direction];
            int nc = j + dc[direction];

            if (!IsValid(nr, nc)) return false; // 범위 초과
            if (board[nr][nc] == WALL) return false; // 벽
        }
    }

    // 다른 기사와의 충돌 여부 확인
    for (int i = r; i < r + h; i++)
    {
        for (int j = c; j < c + w; j++)
        {
            int nr = i + dr[direction];
            int nc = j + dc[direction];

            if (kn_board[nr][nc] != 0 && kn_board[nr][nc] != id) // 자신과 다른 기사
            {
                int other_id = kn_board[nr][nc];
                if (visited[other_id]) continue;
                visited[other_id] = true;
                if (!IsMovePossible(other_id, direction, visited, false)) return false;
            }
		}
	}

    // 이동 가능하므로 이동
    MoveKnight(id, direction);

    // 이동 후 데미지 업데이트 (밀린 기사들에 대한 데미지 업데이트)
    if (!commanded)
        UpdateKnightDmg(id);

    return true;
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (auto const& cmd : commands)
    {
        const auto& [id, direction] = cmd;

        // 사라진 기사는 통과
        if (knights.find(id) == knights.end()) continue;

        // 기사들에 대한 체스판 정보 생성
        kn_board.assign(L + 1, vector<int>(L + 1));
        for (auto const& knight : knights)
        {
			const auto& [kn_id, kn_value] = knight;
			const auto& [r, c, h, w, k, dmg] = kn_value;

            for (int i = r; i < r + h; i++)
            {
                for (int j = c; j < c + w; j++)
                {
					kn_board[i][j] = kn_id;
				}
			}
		}

        // 이동 불가능한 경우 통과
        vector<bool> visited(N + 1, false);
        if (!IsMovePossible(id, direction, visited)) continue;
    }

    for (auto const& knight : knights)
    {
		const auto& [kn_id, kn_value] = knight;
		const auto& [r, c, h, w, k, dmg] = kn_value;

		total_dmg += dmg;
	}

    cout << total_dmg << endl;


    return 0;
}