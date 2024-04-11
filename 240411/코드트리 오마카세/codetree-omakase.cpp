#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

int l; // 초밥 벨트의 길이; 3 <= l <= 1'000'000'000
int q; // 명령의 수; 1 <= q <= 100'000

struct Query {
    int cmd; // 명령 종류
    int t; // 시간
    int x; // 위치
    int n; // 먹는 초밥 수
    string name; // 이름

    // (t, cmd) 순으로 정렬
    bool operator<(const Query& q) const {
		if (t != q.t)
			return t < q.t;
		return cmd < q.cmd;
	}
};

// 명령
vector<Query> queries;

// 사람
set<string> names;

// 각 사람마다 주어진 초밥 명령
map<string, vector<Query>> p_queries;

// 각 사람마다 입장 시간
map<string, int> entry_time;

// 각 손님의 위치
map<string, int> position;

// 각 사람마다 퇴장 시간
map<string, int> exit_time;

// Query를 (t, cmd) 순으로 정렬
bool cmp(Query q1, Query q2)
{
    if (q1.t != q2.t)
        return q1.t < q2.t;
    return q1.cmd < q2.cmd;
}

void Init()
{
    cin >> l >> q;
    for (int i = 0; i < q; i++)
    {
        int cmd = -1;
        int t = -1, x = -1, n = -1;
        string name;

        cin >> cmd >> t;
        if (cmd == 100)
            cin >> x >> name;
        else if (cmd == 200)
            cin >> x >> name >> n;

        queries.push_back({ cmd, t, x, n, name });

        // 사람별 주어진 초밥 목록 관리
        if (cmd == 100)
        {
			p_queries[name].push_back({ cmd, t, x, n, name });
        }
        else if (cmd == 200)
        {
            names.insert(name);
            entry_time[name] = t;
            position[name] = x;
        }
    }
}

void AddQueries()
{
    for (string name : names)
    {
        // 퇴장 시간 관리 (마지막 초밥을 먹는 시간)
        exit_time[name] = 0;

        for (Query qr : p_queries[name])
        {
            int time_to_removed = 0;
            // 초밥이 미리 주어졌다면
            if (qr.t < entry_time[name])
            {
                // entry time 당시 초밥 위치
                int t_sushi_x = (qr.x + (entry_time[name] - qr.t)) % l;
                // 만날 시간 계산
                int additional_time = (position[name] - t_sushi_x + l) % l;

                time_to_removed = entry_time[name] + additional_time;
            }
            // 초밥이 사람 등장 이후에 주어진다면
            else
            {
                int additional_time = (position[name] - qr.x + l) % l;
                time_to_removed = qr.t + additional_time;
            }

            // 가장 늦은 시간 업데이트
            exit_time[name] = max(exit_time[name], time_to_removed);

            // 초밥이 사라지는 111번 쿼리 추가
            queries.push_back({ 111, time_to_removed, -1, -1, name });
        }
    }

    // 사람마다 초밥을 마지막으로 먹은 시간 t를 계산하여 떠나는 쿼리 추가
    for (string name : names)
        queries.push_back({ 222, exit_time[name], -1, -1, name });

    sort(queries.begin(), queries.end());
}

void Simulation()
{
    int people_num = 0, sushi_num = 0;
    for (int i = 0; i < (int)queries.size(); i++) {
        if (queries[i].cmd == 100) // 초밥 추가
            sushi_num++;
        else if (queries[i].cmd == 111) // 초밥 제거
            sushi_num--;
        else if (queries[i].cmd == 200) // 사람 추가
            people_num++;
        else if (queries[i].cmd == 222) // 사람 제거
            people_num--;
        else // 사진 촬영시 답을 출력
            cout << people_num << " " << sushi_num << "\n";
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    Init();
    AddQueries();
    Simulation();

    return 0;
}