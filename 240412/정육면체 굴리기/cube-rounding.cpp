#include <iostream>
#include <vector>
#include <list>
using namespace std;

short n; // 1 <= n <= 20; 지도 크기
short m; // 1 <= m <= 20; 지도 크기
short x; // 0 <= x <= n - 1; 주사위의 위치
short y; // 0 <= y <= m - 1; 주사위의 위치
short k; // 1 <= k <= 1000; 명령의 개수
vector<short> dice(6); // 주사위
vector<vector<short>> map(20, vector<short>(20)); // 지도
vector<short> command(1000); // 명령

short top; // 주사위 윗면

enum {
	TOP = 0,
	BOTTOM,
	RIGHT,
	LEFT,
	FRONT,
	BACK
};

enum {
	EAST = 1,
	WEST,
	NORTH,
	SOUTH
}; // 동, 서, 북, 남

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m >> x >> y >> k;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < k; i++) {
		cin >> command[i];
	}

	for (int i = 0; i < k; i++) {
		vector<short> temp = dice;
		bool valid = true;
		switch (command[i]) {
		case EAST:
			if (y == m - 1) {
				valid = false;
				break;
			}
			y++;
			dice[TOP] = temp[LEFT];
			dice[BOTTOM] = temp[RIGHT];
			dice[LEFT] = temp[BOTTOM];
			dice[RIGHT] = temp[TOP];
			break;
		case WEST:
			if (y == 0) {
				valid = false;
				break;
			}
			y--;
			dice[TOP] = temp[RIGHT];
			dice[BOTTOM] = temp[LEFT];
			dice[LEFT] = temp[TOP];
			dice[RIGHT] = temp[BOTTOM];
			break;
		case NORTH:
			if (x == 0) {
				valid = false;
				break;
			}
			x--;
			dice[TOP] = temp[BACK];
			dice[BOTTOM] = temp[FRONT];
			dice[FRONT] = temp[TOP];
			dice[BACK] = temp[BOTTOM];
			break;
		case SOUTH:
			if (x == n - 1) {
				valid = false;
				break;
			}
			x++;
			dice[TOP] = temp[FRONT];
			dice[BOTTOM] = temp[BACK];
			dice[FRONT] = temp[BOTTOM];
			dice[BACK] = temp[TOP];
			break;
		default:
			break;
		}
		if (valid) {
			if (map[x][y] == 0)
				map[x][y] = dice[BOTTOM];
			else {
				dice[BOTTOM] = map[x][y];
				map[x][y] = 0;
			}
			cout << dice[TOP] << '\n';
		}
	}

	return 0;
}