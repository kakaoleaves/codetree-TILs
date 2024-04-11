#include <iostream>
#include <vector>
#include <unordered_map>

#define MAX_M 10 + 1

using namespace std;

int n, m, q;

// 각 id별로 상자 무게를 저장합니다.
unordered_map<int, int> weight;

// id에 해당하는 상자의 nxt값과 prv값을 관리합니다.
// 0이면 없다는 뜻입니다.
unordered_map<int, int> prv, nxt;

// 각 벨트별로 head, tail id를 관리합니다.
// 0이면 없다는 뜻입니다.
vector<int> head(MAX_M), tail(MAX_M);

// 벨트가 망가졌는지를 표시합니다.
vector<bool> broken(MAX_M);

// 물건 별로 벨트 번호를 기입합니다.
// 벨트 번호가 0이면 사라진 것입니다.
unordered_map<int, int> belt_num;

// 공장 설립
void BuildFactory()
{
	cin >> n >> m;
    vector<int> ids(n), ws(n);
    for (int i = 0; i < n; i++)
        cin >> ids[i];
    for (int i = 0; i < n; i++)
        cin >> ws[i];

    // id마다 무게를 관리합니다.
    for (int i = 0; i < n; i++)
        weight[ids[i]] = ws[i];

    // 벨트 별로 상자 목록을 넣어줍니다.
    int size = n / m;
    for (int i = 1; i <= m; i++)
    {
        // head, tail을 설정해줍니다.
        head[i] = ids[(i - 1) * size];
        tail[i] = ids[i * size - 1];
        for (int j = (i - 1) * size; j < i * size; j++)
        {
            // 상자 ID마다 벨트 번호를 기입합니다.
            belt_num[ids[j]] = i;

            // nxt, prv를 설정해줍니다.
            if (j < i * size - 1)
            {
                nxt[ids[j]] = ids[j + 1];
                prv[ids[j + 1]] = ids[j];
            }
        }
    }
}

// ID에 해당하는 상자를 삭제합니다.
void RemoveBox(int id, bool remove_belt)
{
    int b_num = belt_num[id];

    // 벨트 번호를 제거해야 한다면, 벨트 번호를 제거합니다.
    if (remove_belt)
		belt_num[id] = 0;

    // 하나 남은 원소라면 head, tail이 사라지고 종료
    if (head[b_num] == tail[b_num])
    {
		head[b_num] = tail[b_num] = 0;
	}

    // head를 삭제하는 경우
    else if (head[b_num] == id)
    {
		head[b_num] = nxt[id];
		prv[nxt[id]] = 0;
	}

    // tail을 삭제하는 경우
    else if (tail[b_num] == id)
    {
        tail[b_num] = prv[id];
        nxt[prv[id]] = 0;
    }

    // 중간에 있는 원소를 삭제하는 경우
    else
    {
        nxt[prv[id]] = nxt[id];
        prv[nxt[id]] = prv[id];
    }

    // nxt, prv 값 삭제
    nxt[id] = prv[id] = 0;
}

// target_id 뒤에 id를 추가합니다.
void PushBox(int target_id, int id)
{
    nxt[target_id] = id;
    prv[id] = target_id;

    // tail을 추가하는 경우
    if (tail[belt_num[target_id]] == target_id)
    {
		tail[belt_num[target_id]] = id;
	}
}

void Unload()
{
    int w_max;
    cin >> w_max;

    // 각 벨트마다 첫 번째 상자를 확인합니다.
    long long w_sum = 0;

    for (int i = 1; i <= m; i++)
    {
        // 벨트가 망가졌다면 넘어갑니다.
		if (broken[i])
			continue;

		// 벨트의 첫 번째 상자의 무게를 확인합니다.
		int id = head[i];
        int w = weight[id];

        // 무게가 w_max보다 작다면 무게를 더해주고 상자를 삭제합니다.
        if (w <= w_max)
        {
            // 무게를 더해줍니다.
            w_sum += w;
            
            // 상자를 삭제합니다.
			RemoveBox(id, true);
		}
        // 그렇지 않다면 상자를 맨 뒤로 올려줍니다.
        else if (nxt[id] != 0)
        {
            RemoveBox(id, false);
            PushBox(tail[i], id);
        }
    }

    // 무게를 출력합니다.
    cout << w_sum << endl;
}

void Remove()
{
    int r_id;
    cin >> r_id;

    // 이미 사라진 상자라면 넘어갑니다.
    if (belt_num[r_id] == 0)
    {
        cout << -1 << endl;
        return;
    }

    // 상자를 삭제합니다.
    RemoveBox(r_id, true);
    cout << r_id << endl;
}

void Find()
{
    int f_id;
    cin >> f_id;

    // 이미 사라진 상자라면 넘어갑니다.
    if (belt_num[f_id] == 0)
    {
		cout << -1 << endl;
		return;
	}

    // 상자를 찾아 맨 앞으로 당겨줍니다.
    int b_num = belt_num[f_id];
    if (head[belt_num[f_id]] != f_id)
    {
        int orig_head = head[b_num];
        int orig_tail = tail[b_num];

        // 새로운 tail : f_id 직전
        tail[b_num] = prv[f_id];
        // 새로운 head : f_id
        head[b_num] = f_id;

        // 기존의 tail은 기존의 head와 연결
        prv[orig_head] = orig_tail;
        nxt[orig_tail] = orig_head;

        // 새로운 head, tail에 대한 정보
        prv[f_id] = 0;
        nxt[tail[b_num]] = 0;
	}

	cout << belt_num[f_id] << endl;
}

void HandleBrokenBelt()
{
    int b_num;
    cin >> b_num;

    // 벨트가 이미 망가졌다면 넘어갑니다.
    if (broken[b_num])
    {
		cout << -1 << endl;
		return;
	}

    // 벨트를 망가뜨립니다.
    broken[b_num] = true;

    // 빈 벨트라면 바로 종료합니다.
    if (head[b_num] == 0)
    {
        cout << b_num << endl;
        return;
    }

    // 오른쪽으로 돌면서 망가지지 않은 벨트를 찾아, 옮겨줍니다.
    int nxt_num = b_num;
    while (true)
    {
        nxt_num++;
		if (nxt_num > m)
			nxt_num = 1;

        if (!broken[nxt_num])
        {
            // 빈 벨트라면 그대로 옮겨줍니다.
            if (tail[nxt_num] == 0)
            {
                tail[nxt_num] = tail[b_num];
				head[nxt_num] = head[b_num];
            }

            // 그렇지 않다면 tail에 연결해줍니다.
            else
            {
                // 해당 위치로 상자를 전부 옮겨줍니다.
                PushBox(tail[nxt_num], head[b_num]);
				tail[nxt_num] = tail[b_num];
			}

            // head부터 tail까지 belt_num을 변경해줍니다.
            int cur = head[b_num];
            while (cur != 0)
            {
				belt_num[cur] = nxt_num;
				cur = nxt[cur];
			}
            head[b_num] = tail[b_num] = 0;
            break;
        }
    }
    cout << b_num << endl;
}

int main()
{
    cin >> q;
    while (q--)
    {
        int cmd;
		cin >> cmd;
        switch (cmd)
        {
		case 100:
			BuildFactory();
			break;
		case 200:
			Unload();
			break;
		case 300:
			Remove();
			break;
		case 400:
			Find();
			break;
		case 500:
			HandleBrokenBelt();
			break;
		}
    }
    return 0;
}