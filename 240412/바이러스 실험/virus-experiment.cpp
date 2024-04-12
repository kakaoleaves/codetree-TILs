#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int n; // 1 <= n <= 10; 땅 크기
int m; // 1 <= n <= n^2; 나무의 개수
int k; // 1 <= k <= 1000; 년 수

const int START_NUTRIENT = 5; // 초기 양분
vector<vector<int>> soils(11, vector<int>(11, START_NUTRIENT)); // 땅
vector<vector<int>> a(11, vector<int>(11)); // 겨울에 추가되는 양분
map<pair<int, int>, vector<int>> tree; // 나무의 위치와 나이
map<pair<int, int>, int> dead_tree; // 죽은 나무의 위치와 영양분 정보
map<pair<int, int>, int> breed_tree; // 번식하는 나무의 위치와 개수

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;

    // A 배열의 값 입력
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> a[i][j];
        }
    }

    // 상도가 심은 나무의 정보
    for (int i = 0; i < m; i++)
    {
        int x, y, z;
		cin >> x >> y >> z;
        tree[{x, y}].push_back(z);
    }

    while (k--)
    {
        if (tree.empty()) break;

        // 봄
        for (auto& t : tree)
        {
            auto [coords, ages] = t;
            auto [r, c] = coords;
            sort(ages.begin(), ages.end());

            vector<int> new_ages;
            for (const auto& age : ages)
            {
                if (soils[r][c] >= age)
                {
                    soils[r][c] -= age;
                    new_ages.push_back(age + 1);

                    if ((age + 1) % 5 == 0)
                    {
						breed_tree[{r, c}]++;
					}

                }
                else
                {
                    dead_tree[{r, c}] += age / 2;
                }
            }
            tree[coords] = new_ages;
        }

        // 여름
        for (const auto& d : dead_tree)
        {
			auto [coords, nutrient] = d;
			auto [r, c] = coords;
			soils[r][c] += nutrient;
		}
        dead_tree.clear();

        // 가을
        for (const auto& b : breed_tree)
        {
            const auto& [coords, cnt] = b;
            const auto& [r, c] = coords;

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue; // 자기 자신은 제외
                    int nr = r + i;
                    int nc = c + j;
                    if (nr < 1 || nr > n || nc < 1 || nc > n) continue; // 범위를 벗어나면 제외
                    tree[{nr, nc}].insert(tree[{nr, nc}].begin(), cnt, 1);
                }
            }
        }
        breed_tree.clear();

        // 겨울
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
				soils[i][j] += a[i][j];
			}
		}
    }

    int ans = 0;

    for (auto it = tree.begin(); it != tree.end(); it++)
    {
		ans += it->second.size();
	}

    cout << ans << '\n';

    return 0;
}