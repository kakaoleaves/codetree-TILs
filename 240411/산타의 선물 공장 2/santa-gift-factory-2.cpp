#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_N = 100001;
const int MAX_M = 100001;

int q; // 명령의 수; 1 <= q <= 100'000
int n; // 벨트의 수; 2 <= n <= 100'000
int m; // 물건의 수; 1 <= m <= 100'000

vector<int> heads(MAX_N, -1); // 각 벨트의 head
vector<int> tails(MAX_N, -1); // 각 벨트의 tail
vector<int> counts(MAX_N, 0); // 각 벨트에 존재하는 개수 표기

vector<int> prv(MAX_M, -1); // 각 물건의 이전
vector<int> nxt(MAX_M, -1); // 각 물건의 이후

void BuildFactory()
{
    cin >> n >> m;

    heads.resize(n + 1, -1);
    tails.resize(n + 1, -1);
    counts.resize(n + 1, 0);

    prv.resize(m + 1, -1);
    nxt.resize(m + 1, -1);

    for (int id = 1; id <= m; id++)
    {
        int b_num;
        cin >> b_num;

        // 소속된 벨트 등록, 벨트의 물건 개수 증가
        counts[b_num]++;

        // 해당 벨트에 head가 없다면 head와 tail로 추가
        if (heads[b_num] == -1)
        {
            heads[b_num] = id;
            tails[b_num] = id;
        }
        // 이미 벨트에 물건이 존재하기 때문에 tail만 갱신
        else
        {
            // 현재 tail과 id 연결
            int cur_tail = tails[b_num];
            nxt[cur_tail] = id;
            prv[id] = cur_tail;
            // 벨트의 tail 갱신
            tails[b_num] = id;
        }
    }
}

void MoveAll()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    // src 벨트의 길이가 0이라면 아무것도 하지 않음
    if (counts[m_src] == 0)
    {
        cout << counts[m_dst] << endl;
        return;
    }

    // src 벨트의 head와 tail을 가져옴
    int src_head = heads[m_src];
    int src_tail = tails[m_src];

    // dst 벨트의 head가 존재한다면
    if (counts[m_dst] > 0)
    {
        // dst 벨트의 head에 src 벨트의 tail을 연결
        prv[heads[m_dst]] = src_tail;
        nxt[src_tail] = heads[m_dst];

        // dst 벨트의 head 갱신
        heads[m_dst] = src_head;
    }
    else
    {
        // dst 벨트의 head가 존재하지 않는다면
        tails[m_dst] = src_tail;
        heads[m_dst] = src_head;
    }

    // src 벨트의 head와 tail 초기화
    heads[m_src] = -1;
    tails[m_src] = -1;

    // src 벨트의 개수를 dst 벨트로 이동
    counts[m_dst] += counts[m_src];
    counts[m_src] = 0;

    cout << counts[m_dst] << endl;
}

void SwapFront()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    if (counts[m_src] == 0 && counts[m_dst] == 0)
    {
        cout << counts[m_dst] << endl;
        return;
    }

    else if (counts[m_src] == 0)
    {
        // dst -> src 하나 이동
        int dst_head = heads[m_dst];

        if (counts[m_dst] == 1) // dst 벨트에 물건이 하나만 존재하는 경우
        {
			heads[m_dst] = -1;
			tails[m_dst] = -1;
		}
        else
        {
			heads[m_dst] = nxt[dst_head];
            prv[heads[m_dst]] = -1;
		}

        heads[m_src] = dst_head;
        tails[m_src] = dst_head;
        prv[heads[m_src]] = -1;
        nxt[heads[m_src]] = -1;

        counts[m_src] = 1;
        counts[m_dst] -= 1;
    }

    else if (counts[m_dst] == 0)
    {
		// src -> dst 하나 이동
		int src_head = heads[m_src];

		if (counts[m_src] == 1) // src 벨트에 물건이 하나만 존재하는 경우
		{
            heads[m_src] = -1;
            tails[m_src] = -1;
        }
        else
        {
			heads[m_src] = nxt[src_head];
			prv[heads[m_src]] = -1;
		}

        heads[m_dst] = src_head;
        tails[m_dst] = src_head;
        prv[heads[m_dst]] = -1;
        nxt[heads[m_dst]] = -1;

        counts[m_dst] = 1;
        counts[m_src] -= 1;
    }

    else
    {
        // src <-> dst
        int src_head = heads[m_src];
        int dst_head = heads[m_dst];

        if (counts[m_src] == 1)
        {
            heads[m_src] = dst_head;
            tails[m_src] = dst_head;
            prv[heads[m_src]] = -1;
            nxt[tails[m_src]] = -1;
        }
        else
        {
            heads[m_src] = dst_head;
            prv[heads[m_src]] = -1;
            nxt[heads[m_src]] = nxt[src_head];
            prv[nxt[heads[m_src]]] = heads[m_src];
		}
        
        if (counts[m_dst] == 1)
        {
            heads[m_dst] = src_head;
			tails[m_dst] = src_head;
			prv[heads[m_dst]] = -1;
			nxt[tails[m_dst]] = -1;
        }
        else
        {
            heads[m_dst] = src_head;
			prv[heads[m_dst]] = -1;
			nxt[heads[m_dst]] = nxt[dst_head];
			prv[nxt[heads[m_dst]]] = heads[m_dst];
        }
    }

    cout << counts[m_dst] << endl;
}

void Divide()
{
	int m_src, m_dst;
	cin >> m_src >> m_dst;

    if (counts[m_src] <= 1)
    {
		cout << counts[m_dst] << endl;
		return;
	}

    int cnt = counts[m_src] / 2;

    int tmp_head = heads[m_src];
    int tmp_tail = heads[m_src];

    for (int i = 0; i < cnt - 1; i++)
        tmp_tail = nxt[tmp_tail];

    heads[m_src] = nxt[tmp_tail];
    prv[heads[m_src]] = -1;
    nxt[tmp_tail] = -1;

    if (counts[m_dst] == 0)
    {
        // dst 벨트에 물건이 없는 경우
        heads[m_dst] = tmp_head;
        tails[m_dst] = tmp_tail;
        prv[heads[m_dst]] = -1;
        nxt[tails[m_dst]] = -1;
    }
    else
    {
        // dst 벨트에 물건이 있는 경우
        prv[heads[m_dst]] = tmp_tail;
        nxt[tmp_tail] = heads[m_dst];

        heads[m_dst] = tmp_head;
        prv[heads[m_dst]] = -1;
    }

    counts[m_dst] += cnt;
    counts[m_src] -= cnt;

    cout << counts[m_dst] << endl;   
}

void GetGiftInfo()
{
    int p_num;
    cin >> p_num;

    cout << prv[p_num] + 2 * nxt[p_num] << endl;
}

void GetBeltInfo()
{
    int b_num;
    cin >> b_num;

    cout << heads[b_num] + 2 * tails[b_num] + 3 * counts[b_num] << endl;
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int cmd;
        cin >> cmd;
        switch (cmd)
        {
        case 100:
            BuildFactory();
            break;
        case 200:
            MoveAll();
            break;
        case 300:
            SwapFront();
            break;
        case 400:
            Divide();
            break;
        case 500:
            GetGiftInfo();
            break;
        case 600:
            GetBeltInfo();
            break;
        }
    }
    return 0;
}