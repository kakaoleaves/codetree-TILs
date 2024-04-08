#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int l; // 초밥 벨트의 길이, 3 <= l <= 1'000'000'000
int q; // 명령의 수, 1 <= q <= 100'000

struct Command
{
    int cmd;
    int t;
    int x;
    string name;
    int n;
};

unordered_multimap<string, pair<int, int>> sushis;
unordered_map<string, pair<int, int>> customers;

vector<Command> commands;

int cur_cmd = 0;
int cur_t = 0;

void Input()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> l >> q;

    commands.resize(q);

    for (int i = 0; i < q; i++)
    {
        int cmd, t, x, n;
        string name;
        Command command;
        cin >> cmd >> t;

        if (cmd == 300)
        {
            command = { cmd, t, 0, "", 0 };
        }
        else if (cmd == 200)
        {
            cin >> x >> name >> n;
            command = { cmd, t, x, name, n };
        }
        else if (cmd == 100)
        {
            cin >> x >> name;
            command = { cmd, t, x, name, 0 };
        }

        commands[i] = command;
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Input();

    while (cur_cmd < q)
    {
        // 명령 처리 전에 손님이 먹기
        if (!sushis.empty() && !customers.empty())
        {
            vector<string> customers_to_erase;
            for (auto it = customers.begin(); it != customers.end(); it++)
            {
                auto& [name, cinfo] = *it;
                auto& [cx, cn] = cinfo;
                auto [start, end] = sushis.equal_range(name);

                if (distance(start, end) == 0) continue;

                // 초밥을 가까운 순으로 정렬
                vector<unordered_multimap<string, pair<int, int>>::iterator> _sushis;
                vector<pair<int, int>> csx_sushis; // {csx, idx}
                for (auto itt = start; itt != end; itt++)
                {
                    const auto& [_, sinfo] = *itt;
                    const auto& [sx, st] = sinfo;
                    int csx = (cur_t - st + sx) % l;
                    _sushis.push_back(itt);
                    csx_sushis.push_back({ (cx - csx + l) % l, _sushis.size() - 1 });
                }
                sort(csx_sushis.begin(), csx_sushis.end());

                int gap = commands[cur_cmd].t - cur_t;

                for (const auto& csx_sushi : csx_sushis)
                {
					const auto& [csx, idx] = csx_sushi;
                    if (csx <= gap)
                    {
						sushis.erase(_sushis[idx]);
						cn--;
                        if (cn == 0)
                        {
							customers_to_erase.push_back(name);
							break;
						}
					}
					else break;
				}
            }

            if (!customers_to_erase.empty())
            {
                for (const string& name : customers_to_erase)
                {
					customers.erase(name);
				}
			}
        }

        cur_t = commands[cur_cmd].t;

        // 명령 처리
        if (Command& cur_command = commands[cur_cmd]; cur_command.cmd == 300)
        {
            cout << customers.size() << ' ' << sushis.size() << '\n';
        }
        else if (cur_command.cmd == 200)
        {
            // 손님 추가
            auto [cmd, t, x, name, n] = cur_command;

            // 이름이 같은 초밥을 찾는다.
            auto [start, end] = sushis.equal_range(name);
            if (distance(start, end) == 0) continue;

            // 초밥을 가까운 순으로 정렬
            vector<unordered_multimap<string, pair<int, int>>::iterator> _sushis;
            vector<pair<int, int>> csx_sushis; // {csx, idx}
            for (auto itt = start; itt != end; itt++)
            {
                const auto& [_, sinfo] = *itt;
                const auto& [sx, st] = sinfo;
                int csx = (cur_t - st + sx) % l;
                _sushis.push_back(itt);
                csx_sushis.push_back({ (x - csx + l) % l, _sushis.size() - 1 });
            }
            sort(csx_sushis.begin(), csx_sushis.end());

            for (const auto& csx_sushi : csx_sushis)
            {
                const auto& [csx, idx] = csx_sushi;
                if (csx == 0)
                {
                    sushis.erase(_sushis[idx]);
					n--;
					if (n == 0) break;
                }
                else break;
            }
			if (n > 0) customers.try_emplace(name, x, n);
        }
        else if (cur_command.cmd == 100)
        {
            const string& name = cur_command.name;
            if (auto it = customers.find(name); it != customers.end())
            {
                auto& [_, cinfo] = *it;
                auto& [cx, cn] = cinfo;

                if (cx == cur_command.x)
                {
                    cn--;
                    if (cn == 0) customers.erase(name);
                }
                else
                {
                    sushis.emplace(name, make_pair(cur_command.x, cur_t));
                }
            }
            else
            {
                sushis.emplace(name, make_pair(cur_command.x, cur_t));
            }
        }
        cur_cmd++;
    }
    return 0;
}