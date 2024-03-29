#include <iostream>
#include <vector>

using namespace std;

int n;  // 게임판의 크기
int m;  // 게임 턴 수
int p;  // 산타의 수
int c;  // 루돌프의 힘
int d;  // 산타의 힘
pair<int, int> r; // 루돌프의 위치

vector<pair<int, int>> santas(31);
vector<vector<int>> grid(51, vector<int>(51, 0));

vector<bool> stunned(31, false);
vector<int> stunned_turns(31, 0);
vector<bool> survived(31, true);
vector<int> score(31, 0);

const vector<int> dx = { -1, 0, 1, 0 };
const vector<int> dy = { 0, 1, 0, -1 };

bool IsValid(int r, int c)
{
    return (r >= 1 && r <= n && c >= 1 && c <= n);
}

int main() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> p >> c >> d;

    cin >> r.first >> r.second;

    for (int i = 1; i <= p; i++)
    {
        int num, sr, sc;
        cin >> num >> sr >> sc;
        santas[num] = { sr, sc };
    }

    /*지워야됨 */
    // m = 4;

    while (m--)
    {
        // 게임 가능 여부 확인 : P명 모두 탈락했는지 조회
        bool possible = false;
        for (int i = 1; i <= p; i++)
        {
            if (survived[i]) possible = true;
        }
        if (!possible) break;

        // 게임판 구성
        vector<vector<int>> grid(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= p; i++)
        {
            if (survived[i])
            {
                auto& [sr, sc] = santas[i];
                grid[sr][sc] = i;
            }
        }

        // 기절한 산타 초기화
        for (int i = 1; i <= p; i++)
        {
            if (stunned_turns[i] > 0) {
                stunned_turns[i]--;
                if (stunned_turns[i] == 0) stunned[i] = false;
            }
        }

        // 루돌프 움직임 결정
        int nxt_santa = -1; // 가장 가까운 산타의 번호
        int nxt_dist = -1;  // 가장 가까운 산타와의 거리
        auto& [rr, rc] = r;

        for (int i = 1; i <= p; i++)
        {
            if (survived[i])
            {
                auto& [sr, sc] = santas[i];
                int dist = (rr - sr) * (rr - sr) + (rc - sc) * (rc - sc);

                if (nxt_santa == -1) // 첫 번째 생존 산타로 초기화
                {
                    nxt_santa = i;
                    nxt_dist = dist;
                    continue;
                }
                else
                {
                    auto& [nsr, nsc] = santas[nxt_santa];
                    if ((dist < nxt_dist) || (dist == nxt_dist && (sr > nsr || (sr == nsr && sc > nsc))))
                    {
                        // 가장 가까운 산타, 같다면 r, c가 큰 산타
                        nxt_santa = i;
                        nxt_dist = dist;
                    }
                }

            }
        }

        // cout << nxt_santa << endl;

        int rdx = 0;    // 루돌프의 움직임 가로 방향
        int rdy = 0;    // 루돌프의 움직임 세로 방향

        // 가장 가까운 산타에게 루돌프 돌진
        if (nxt_santa != -1)
        {
            auto& [sr, sc] = santas[nxt_santa];

            if (rr > sr) rdy--;
            else if (rr < sr) rdy++;

            if (rc > sc) rdx--;
            else if (rc < sc) rdx++;

            rr += rdy;
            rc += rdx;

            if (grid[rr][rc] != 0)  // 루돌프가 움직여서 충돌 발생
            {
                int coll = grid[rr][rc];
                score[coll] += c;        
                stunned[coll] = true;
                stunned_turns[coll] = 2;    // 루돌프와 충돌한 산타에 대한 처리(점수와 기절)

                auto& [csr, csc] = santas[coll];
                grid[csr][csc] = 0; // 충돌해서 산타가 이동했으므로 GRID의 값 변경

                csr += c * rdy; csc += c * rdx; // 산타 정보에서 좌표 갱신

                if (IsValid(csr, csc)) {
                    while (true) // 산타들의 연쇄 충돌 계산
                    {
                        auto [ccsr, ccsc] = santas[coll]; // 복사본
                        if (grid[ccsr][ccsc] == 0) // 연쇄 충돌 X
                        {
                            grid[ccsr][ccsc] = coll;
                            break;
                        }
                        // 연쇄 충돌 O
                        int nxt_coll = grid[ccsr][ccsc];
                        grid[ccsr][ccsc] = coll;

                        auto& [ncsr, ncsc] = santas[nxt_coll];
                        ncsr += rdy; ncsc += rdx;

                        if (!IsValid(ncsr, ncsc)) break;
                        coll = nxt_coll;
                    }
                }
                else
                {
                    survived[coll] = false;
                }
            }
        }

        // cout << "[rr, rc] : " << rr << ", " << rc << endl;

        // 산타들의 이동
        for (int i = 1; i <= p; i++)
        {
            if (survived[i] && !stunned[i])
            {
                // cout << "Santa " << i << " is moving!" << endl;
                auto& [sr, sc] = santas[i];
                int nd, ni;
                nd = ni = -1;

                for (int j = 0; j < 4; j++) // 좌하우상 순서로 조회하면 상우하좌의 우선순위를 지킬 수 있음
                {
                    int candi_sr = sr + dy[j];
                    int candi_sc = sc + dx[j];

                    if (!IsValid(candi_sr, candi_sc)) continue;
                    if (grid[candi_sr][candi_sc] != 0) continue;

                    // 루돌프에게 가까워지는 방향 여부 조회
                    int candi_d = (rr - candi_sr) * (rr - candi_sr) + (rc - candi_sc) * (rc - candi_sc);
                    int d = (rr - sr) * (rr - sr) + (rc - sc) * (rc - sc);
                    if (candi_d < d && (nd == -1 || candi_d <= nd))
                    // 루돌프와 가까워지는 방향 + 구한 값이 지금까지 계산한 것보다 최소일 때 갱신
                    {
                        // cout << "이걸로 정했다!" << j << " [" << candi_sr << ", " << candi_sc << "] : " << grid[candi_sr][candi_sc] << endl;
                        nd = candi_d;
                        ni = j;
                    }
                }

                if (nd != -1 && ni != -1) // 산타가 이동 가능하다면,
                {
                    grid[sr][sc] = 0;
                    // cout << "산타 " << i << "가 [" << ni << "] 방향으로 이동" << endl;
                    sr += dy[ni]; sc += dx[ni];
                    // cout << "산타 " << i << "가 [" << sr << ", " << sc << "]로 이동" << endl;
                    grid[sr][sc] = i;

                    if (sr == rr && sc == rc) // 루돌프와 충돌했다면
                    {
                        // cout << "산타 " << i << "가 루돌프와 충돌!!" << endl;
                        int coll = i; // 충돌한 산타를 coll로 배정
                        score[coll] += d;
                        stunned[coll] = true; 
                        stunned_turns[coll] = 2;    // 점수와 기절 처리

                        grid[sr][sc] = 0; // 충돌해서 산타가 이동하므로 GRID의 값 변경

                        ni = (ni + 2) % 4;
                        sr += d * dy[ni]; sc += d * dx[ni];

                        if (IsValid(sr, sc))
                        {
                            while (true)
                            {
                                auto [ccsr, ccsc] = santas[coll];
                                if (grid[ccsr][ccsc] == 0) // 연쇄 충돌 X
                                {
                                    grid[ccsr][ccsc] = coll;
                                    break;
                                }
                                // 연쇄충돌 O
                                int nxt_coll = grid[ccsr][ccsc];
                                grid[ccsr][ccsc] = coll;

                                auto& [ncsr, ncsc] = santas[nxt_coll];
                                ncsr += dy[ni]; ncsc += dx[ni];

                                if (!IsValid(ncsr, ncsc)) break;
                                coll = nxt_coll;
                            }
                        }
                        else
                        {
                            survived[coll] = false;
                        }
                    }
                }

                // for (int i = 1; i <= n; i++)
                // {
                //     for (int j = 1; j <= n; j++)
                //     {
                //         cout << grid[i][j] << ' ';
                //     }
                //     cout << '\n';
                // }

                // cout << "santa " << i << " [r, c] : " << sr << ", " << sc << endl;
            }
        }

        // 탈락하지 않은 산타들에게 점수 추가
        for (int i = 1; i <= p; i++)
        {
            if (survived[i]) score[i]++;
        }
    }

    for (int i = 1; i <= p; i++)
    {
        cout << score[i] << ' ';
    }

    return 0;
}