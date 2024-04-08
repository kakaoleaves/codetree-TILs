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

int main() {
    // 여기에 코드를 작성해주세요.

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

    while (cur_cmd < q)
    {
        cur_t++;

        // 손님이 먹기
        vector<string> customersToErase;

        for (auto it = customers.begin(); it != customers.end(); it++)
        {
            vector<unordered_multimap<string, pair<int, int>>::iterator> sushisToErase;

            auto& [name, cinfo] = *it;
            auto& [cx, cn] = cinfo;
            auto [start, end] = sushis.equal_range(name);

            for (auto itt = start; itt != end; itt++)
            {
                const auto& [_, sinfo] = *itt;
                const auto& [sx, st] = sinfo;
                int csx = (cur_t - st + sx) % l;
                if (csx == cx)
                {
                    cn--;
                    // cout << cur_t << ": " << name << "가 먹었다!" << endl;
                    sushisToErase.push_back(itt);

                    if (cn == 0)
                    {
                        customersToErase.push_back(name);
                        break;
                    }
                }
            }

            for (const auto& itt : sushisToErase)
            {
				sushis.erase(itt);
			}
        }

        for (const auto& name : customersToErase)
        {
			customers.erase(name);
		}


        if (commands[cur_cmd].t == cur_t)
        {
            if (Command& cur_command = commands[cur_cmd]; cur_command.cmd == 300)
            {
                cout << customers.size() << ' ' << sushis.size() << '\n';
            }
            else if (cur_command.cmd == 200)
            {
                auto [cmd, t, x, name, n] = cur_command;
                // 이름이 같은 초밥을 찾는다.
                auto [start, end] = sushis.equal_range(name);
                vector<unordered_multimap<string, pair<int, int>>::iterator> toErase;
                for (auto it = start; it != end; it++)
                {
                    const auto& [_, sinfo] = *it;
                    const auto& [sx, st] = sinfo;

                    int csx = (cur_t - st + sx) % l;
                    if (csx == x)
                    {
						// cout << cur_t << ": " << name << "가 먹었다!" << endl;
                        toErase.push_back(it);
                        n--;
						if (n == 0) break;
					}
                }

                for (const auto& it : toErase)
                {
					sushis.erase(it);
				}

				if (n > 0)
                    customers.try_emplace(name, x, n);
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
                        // cout << cur_t << ": " << name << "가 먹었다!" << endl;
                        if (cn == 0) customers.erase(name);
                    }
                    else
                    {
                        sushis.insert({
                            name,
                            {
                                cur_command.x,
                                cur_t
                            }
                            });
                    }
                }
                else
                {
                    sushis.insert({
                    name,
                    {
                       cur_command.x,
                        cur_t
                    }
                    });
                }
            }

            cur_cmd++;
        }


    }


    return 0;
}