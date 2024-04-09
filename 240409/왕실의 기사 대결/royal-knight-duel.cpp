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

int ans = 0;

vector<vector<int>> board(41, vector<int>(41));
map<int, Knight> knights;
vector<pair<int, int>> commands(100);

enum { UP, RIGHT, DOWN, LEFT };

const vector<int> dr = { -1, 0, 1, 0 };
const vector<int> dc = { 0, 1, 0, -1 };

void init()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> L >> N >> Q;

    board.resize(L + 1, vector<int>(L + 1));
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
}

bool isOverlap(const Knight& a, const Knight& b)
{
    if (a.r + a.h <= b.r) return false;
	if (b.r + b.h <= a.r) return false;
	if (a.c + a.w <= b.c) return false;
	if (b.c + b.w <= a.c) return false;
	return true;
}

bool isMovable(int id, int dir, vector<bool>& movable)
{
    const auto& kn = knights[id];
	const auto& [r, c, h, w, k, dmg] = kn;

	int nr = r + dr[dir];
	int nc = c + dc[dir];

    // 경계를 벗어나는 경우
	if (nr < 1 || nr + h - 1 > L || nc < 1 || nc + w - 1 > L) return false;

    // 벽이 있는 경우
    for (int i = nr; i < nr + h; i++)
    {
        for (int j = nc; j < nc + w; j++)
        {
			if (board[i][j] == WALL) return false;
		}
	}

    Knight tmp = { nr, nc, h, w, k, dmg };

    // 다른 기사와 겹치는 경우
    for (const auto& other : knights)
    {
        auto& [oid, okn] = other;
        if (oid == id) continue;
        if (isOverlap(tmp, okn) && !isMovable(oid, dir, movable)) return false;
	}

    movable[id] = true;
	return true;
}

void moveKnight(int id, int dir)
{
    if (knights.find(id) == knights.end()) return;

    auto& kn = knights.at(id);
    auto& [r, c, h, w, k, dmg] = kn;

    r += dr[dir];
    c += dc[dir];
}

void postMoveProcessing(vector<int>& dmged)
{
    for (const auto& id : dmged)
    {
        if (knights.find(id) == knights.end()) continue;

        auto& kn = knights.at(id);
        auto& [r, c, h, w, k, dmg] = kn;

        for (int i = r; i < r + h; i++)
        {
            for (int j = c; j < c + w; j++)
            {
                if (board[i][j] == TRAP)
                {
                    dmg++;
                    k--;
                    if (k == 0)
                    {
                        knights.erase(id);
                        break;
                    }
                }
			}
		}
	}

    dmged.clear();
}

int main() {
    // 여기에 코드를 작성해주세요.
    init();

    for (auto const& cmd : commands)
    {
        const auto& [id, direction] = cmd;

        // 사라진 기사는 무시
        if (knights.find(id) == knights.end()) continue;

        // 이동 가능한지 확인
        vector<bool> movable(N + 1, false);
        if (!isMovable(id, direction, movable)) continue;


        // 이동
        for (int i = 1; i <= N; i++)
        {
            if (movable[i])
            {
                 moveKnight(i, direction);
            }
        }

        // 함정 처리
        vector<int> dmged;
        for (int i = 1; i <= N; i++)
        {
			if (movable[i] && i != id) dmged.push_back(i);
		}
		postMoveProcessing(dmged);
    }

    for (auto const& knight : knights)
    {
        const auto& [id, kn] = knight;
        const auto& [r, c, h, w, k, dmg] = kn;
        ans += dmg;
    }

    cout << ans << endl;

    return 0;
}