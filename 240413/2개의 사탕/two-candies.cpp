#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>

#define MAX_N 10
#define MAX_M 10

using namespace std;

// 전역 변수 선언:
int n, m;
char a[MAX_N][MAX_M];

// 빨간색, 파란색 사탕의 위치를 저장할 변수입니다. 
pair<int, int> red_pos;  
pair<int, int> blue_pos;


// bfs에 필요한 변수들 입니다.
queue<pair<pair<int, int>, pair<int, int> > > q;
bool visited[MAX_N + 1][MAX_M + 1][MAX_N + 1][MAX_M + 1];
int step[MAX_N + 1][MAX_M + 1][MAX_N + 1][MAX_M + 1];

// MAP 밖으로 나왔다는 의미로 
// 편의상 절대 맵 안에서는 될 수 없는 위치인 (MAX_N, MAX_M)로 설정했습니다.
const pair<int, int> OUT_OF_MAP = make_pair(MAX_N, MAX_M);

int ans = INT_MAX;

// 파란색이 맵에 남아있는지 판단합니다.
bool BlueExist() {
    return blue_pos != OUT_OF_MAP;
}

// 빨간색이 맵에 남아있는지 판단합니다.
bool RedExist() {
    return red_pos != OUT_OF_MAP;
}

// 상자를 dir방향으로 기울였을 때
// 파란색 보다 빨간색을 무조건 먼저 움직여야 하는지 판단합니다.
bool RedMustFirst(int dir) {
    int rx = red_pos.first, ry = red_pos.second;
    int bx = blue_pos.first, by = blue_pos.second;

    if(dir == 0)
        return (ry == by && rx < bx);
    else if(dir == 1)
        return (ry == by && rx > bx);
    else if(dir == 2)
        return (rx == bx && ry < by);
    else
        return (rx == bx && ry > by);
}

// (x, y)로 진행이 가능한지 판단합니다.
// 더 진행하기 위해서는 해당 위치에 벽이나 사탕이 없어야 합니다.
bool CanGo(int x, int y) {
    return a[x][y] != '#' && make_pair(x, y) != red_pos 
		   && make_pair(x, y) != blue_pos;
}

// pos(x, y)에서 dir 방향으로 이동했을 때 
// 최종적으로 도착하는 위치를 반환합니다.
pair<int, int> GetDestination(pair<int, int> pos, int dir) {
    int dx[4] = {-1, 1,  0, 0};
    int dy[4] = { 0, 0, -1, 1};

    int nx = pos.first + dx[dir], ny = pos.second + dy[dir];

    if(!CanGo(nx, ny))
        return pos;
    
    if(a[nx][ny] == 'O')
        return OUT_OF_MAP;

    return GetDestination(make_pair(nx, ny), dir);
}

// dir 0, 1, 2, 3는 각각 상하좌우를 의미합니다.
void Tilt(int dir) {
    if(RedMustFirst(dir)) {
        red_pos = GetDestination(red_pos, dir);
        blue_pos = GetDestination(blue_pos, dir);
    }
    else {
        blue_pos = GetDestination(blue_pos, dir);
        red_pos = GetDestination(red_pos, dir);
    }
}

// 새로운 (빨간색 사탕, 파란색 사탕) 정보를 추가합니다. 
void Push(pair<int, int> red_pos, pair<int, int> blue_pos, int new_step) {
    q.push(make_pair(red_pos, blue_pos));
    visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = true;
    step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = new_step;
}

// BFS를 통해 최소 이동 횟수를 구합니다.
void FindMin() {
    while(!q.empty()) {
        red_pos = q.front().first;
        blue_pos = q.front().second;
        int curr_step = step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second];

        q.pop();

        // 이미 둘 중 하나의 사탕이 상자 안에 없다면 더 이상 진행할 필요가 없습니다.
        if(!BlueExist() || !RedExist())
            continue;

        // 4 방향 중 한 방향을 정하여 움직입니다.
        for(int dir = 0; dir < 4; dir++) {
            // Tilt를 하면 blue, red 사탕의 위치가 바뀌게 되므로, 
			// tilt전 위치를 저장해 놓습니다.
            pair<int, int> temp_red = red_pos;
            pair<int, int> temp_blue = blue_pos;
            
            // dir 방향으로 기울여 사탕의 위치를 변경합니다.
            Tilt(dir);
            
            // 아직 방문한 적이 없는 상태라면, 새로 queue에 넣어줍니다.
            if(!visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second])
                Push(red_pos, blue_pos, curr_step + 1);

            // Tilt 전 상태로 복원하여 그 다음 방향으로도 동일한 기회를
            // 주도록 합니다.
            red_pos = temp_red;
            blue_pos = temp_blue;
        }
    }

    // 빨간색은 상자 밖으로 나갔지만 파란색은 아직 상자 안에 있는 경우 중
    // 최솟값을 구해줍니다.
    for(int bx = 0; bx < n; bx++)
        for(int by = 0; by < m; by++)
            if(visited[OUT_OF_MAP.first][OUT_OF_MAP.second][bx][by])
                ans = min(ans, 
						  step[OUT_OF_MAP.first][OUT_OF_MAP.second][bx][by]);
}

int main() {
    // 입력:
    cin >> n >> m;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++) {
            cin >> a[i][j];

            // 사탕의 경우 위치를 저장해두고, 맵에서는 지워줍니다.
            if(a[i][j] == 'R') {
                a[i][j] = '.';
                red_pos = make_pair(i, j);
            }
            if(a[i][j] == 'B') {
                a[i][j] = '.';
                blue_pos = make_pair(i, j);
            }
        }
    
    // bfs를 이용해 최소 이동 횟수를 구합니다.
    Push(red_pos, blue_pos, 0);
    FindMin();

    // 출력:
    if(ans > 10) // 10번 이내로 답을 구할 수 없다면
        ans = -1; // -1을 답으로 넣어줍니다.
    
    cout << ans;

    return 0;
}