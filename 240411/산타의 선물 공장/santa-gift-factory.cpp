#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <map>

using namespace std;

int q; // 명령의 수; 1 <= q <= 100'000
int n; // 선물의 수; 1 <= n <= 100'000
int m; // 벨트의 수; 1 <= m <= 10, n은 항상 m의 배수
int w_max; // 1 <= w <= 1'000'000'000

struct Box
{
    int b_id;
    int w;
};

vector<pair<int, int>> queries(100000);
map<int, list<Box>> belts;

void Init()
{
    cin >> q;
    queries.resize(q);

    for (int i = 0; i < q; i++)
    {
        int cmd;
        cin >> cmd;

        if (cmd == 100)
        {
            int n, m;
            cin >> n >> m;
            vector<int> ids(n);
            vector<int> weights(n);

            for (int i = 0; i < n; i++)
                cin >> ids[i];
            for (int i = 0; i < n; i++)
                cin >> weights[i];

            int idx = 0;

            for (int i = 1; i <= m; i++)
            {
                for (int j = 0; j < n / m; j++)
                {
                    belts[i].push_back({ ids[idx], weights[idx] });
                    idx++;
                }
            }
        }
        else
        {
            int num;
            cin >> num;
            queries[i] = { cmd, num };
        }
    }
}

void UnloadBox(int w_max)
{
    int unloaded = 0;
    for (auto& m_belt : belts)
    {
        auto& belt = m_belt.second;
        if (belt.empty()) continue;

        // 벨트의 맨 앞이 w_max 이하라면 하차
        auto front_box = belt.front();
        belt.pop_front();

        if (front_box.w <= w_max)
        {
            unloaded += front_box.w;
        }
        // 아니라면 맨 뒤로 보냄
        else
        {
            belt.push_back(front_box);
        }
    }

    cout << unloaded << '\n';
}

void RemoveBox(int r_id)
{
    for (auto& m_belt : belts)
    {
        auto& belt = m_belt.second;
        auto it = find_if(belt.begin(), belt.end(), [&](Box b) { return b.b_id == r_id; });
        if (it != belt.end())
        {
            belt.erase(it);
            cout << r_id << '\n';
            return;
        }
    }
    // 그러한 상자가 없다면 -1 출력
    cout << -1 << '\n';
}

void FindBox(int f_id)
{
    for (auto& m_belt : belts)
    {
        auto& b_num = m_belt.first;
        auto& belt = m_belt.second;
        auto it = find_if(belt.begin(), belt.end(), [&](Box b) { return b.b_id == f_id; });
        if (it != belt.end())
        {
            list<Box> tmp;
            tmp.splice(tmp.begin(), belt, it, belt.end());
            belt.splice(belt.begin(), tmp);

            cout << b_num << '\n';
            return;
        }
    }
    cout << -1 << '\n';
}

void HandleBeltFailure(int b_num)
{
    auto it = belts.find(b_num);
    if (it != belts.end())
    {        
        auto& next_belt = ((next(it) == belts.end()) ? belts.begin() : next(it))->second;
        next_belt.splice(next_belt.end(), it->second);
		belts.erase(it);
        cout << b_num << '\n';
	}
    else
    {
		cout << -1 << '\n';
	}
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();

    for (int i = 1; i < q; i++)
    {
        auto& query = queries[i];
        switch (query.first)
        {
		case 200:
			UnloadBox(query.second);
			break;
		case 300:
			RemoveBox(query.second);
			break;
		case 400:
			FindBox(query.second);
			break;
		case 500:
			HandleBeltFailure(query.second);
			break;
		}
    }

    return 0;
}