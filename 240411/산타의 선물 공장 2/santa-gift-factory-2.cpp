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

vector<int> prv(MAX_M, -1); // 각 물건의 이전
vector<int> nxt(MAX_M, -1); // 각 물건의 이후
vector<int> counts(MAX_M, 0); // 각 벨트에 존재하는 개수 표기

void BuildFactory()
{
    cin >> n >> m;

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

    // m_src가 비어있다면 옮길 필요 없음
    if (heads[m_src] == -1)
    {
        cout << counts[m_dst] << endl;
        return;
    }

    // 아니라면 m_src에 있는 모든 물건을 m_dst 앞으로 옮겨야함

    // 1. head 변경 2. tail 변경 4. count 변경

    // dst의 head, tail에 대한 작업
    // m_dst에 물건이 없었다면, head와 tail이 그대로 따라옴
    if (heads[m_dst] == -1)
    {
		tails[m_dst] = tails[m_src];
        heads[m_dst] = heads[m_src];
    }

    // dst에 물건이 있었다면, src의 tail과 dst의 head를 연결 
    else
    {
        // src의 tail과 dst의 head를 연결
        int cur_dst_head = heads[m_dst];
        int cur_src_tail = tails[m_src];

		nxt[cur_src_tail] = cur_dst_head;
        prv[cur_dst_head] = cur_src_tail;

        heads[m_dst] = heads[m_src];
    }

    // src의 head, tail 변경
    heads[m_src] = tails[m_src] = -1;

    // count 변경
    counts[m_dst] += counts[m_src];
    counts[m_src] = 0;

    cout << counts[m_dst] << endl;
}

// src, dst의 head끼리 swap
void SwapFront()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    // 둘 다 비어 있는 경우
    if (heads[m_src] == -1 && heads[m_dst] == -1)
    {
        cout << counts[m_dst] << endl;
		return;
	}

    // src가 비어있는 경우
    else if (heads[m_src] == -1)
    {
        // dst의 head가 src로 이동한다
        heads[m_src] = tails[m_src] = heads[m_dst];

        // dst의 head는 기존 head의 다음이 된다

        // 만약 dst의 head가 tail이라면 tail도 변경
        if (heads[m_dst] == tails[m_dst])
        {
            heads[m_dst] = -1;
            tails[m_dst] = -1;
		}
        else
        {
            heads[m_dst] = nxt[heads[m_dst]];
            prv[heads[m_dst]] = -1; // head가 되었으므로 이전은 없음
        }

        // src의 head가 되었으므로 이전, 이후 모두 없음
        nxt[heads[m_src]] = -1;


        // count 변경
        counts[m_src]++;
        counts[m_dst]--;
    }

    // dst가 비어있는 경우
    else if (heads[m_dst] == -1)
    {
		// src의 head가 dst로 이동한다
        heads[m_dst] = tails[m_dst] = heads[m_src];

        // src의 head는 기존 head의 다음이 된다

        // 만약 src의 head가 tail이라면 tail도 변경
        if (heads[m_src] == tails[m_src])
        {
			heads[m_src] = -1;
			tails[m_src] = -1;
		}
        else
        {
			heads[m_src] = nxt[heads[m_src]];
			prv[heads[m_src]] = -1; // head가 되었으므로 이전은 없음
		}

        // dst의 head가 되었으므로 이전, 이후 모두 없음
        nxt[heads[m_dst]] = -1;

        // 소속 변경 (하나만 해도 된다)
        int cur = heads[m_dst];

        // count 변경
        counts[m_src]--;
        counts[m_dst]++;
	}

    // 둘 다 비어있지 않은 경우
    else
    {
        int src_head = heads[m_src];
        int dst_head = heads[m_dst];

        // 서로의 head 변경
        heads[m_src] = dst_head;
        heads[m_dst] = src_head;

        // 서로의 head의 이후 변경 (이전은 변경할 필요 없음)
        int src_nxt = nxt[src_head];
        int dst_nxt = nxt[dst_head];

        nxt[src_head] = dst_nxt;
        nxt[dst_head] = src_nxt;

        // 서로의 head가 tail이라면 tail도 변경
        if (src_head == tails[m_src])
        {
			tails[m_src] = dst_head;
            nxt[dst_head] = -1; // tail이 되었으므로 이후는 X
        }

        if (dst_head == tails[m_dst])
		{
            tails[m_dst] = src_head;
            nxt[src_head] = -1; // tail이 되었으므로 이후는 X
        }
        // count는 그대로
    }
    
    cout << counts[m_dst] << endl;
}

void Divide()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    // m_src 내 선물 개수가 1개 이하라면 옮기지 않음
    if (counts[m_src] <= 1)
    {
        cout << counts[m_dst] << endl;
        return;
    }

    // 옮겨질 개수
    int cnt = counts[m_src] / 2;

    // 개수만큼 count 갱신
    counts[m_dst] += cnt;
    counts[m_src] -= cnt;

    // 최종 cur은 floor(n / 2) 다음의 물건, 즉 이 부분이 src의 head가 된다.
    int cur = heads[m_src];
    for (int i = 0; i < cnt; i++)
    {
		cur = nxt[cur];
	}

    // dst의 head 변경
    // dst가 비어있다면 head와 tail 모두 변경
    if (heads[m_dst] == -1)
    {
        heads[m_dst] = heads[m_src];
        tails[m_dst] = prv[cur];        
        nxt[prv[cur]] = -1; // tail이 되었으므로 이후는 X
	}

    // dst가 비어있지 않다면
    else
    {
        // 옮긴 src의 head는 dst의 head
        // 옮긴 src의 tail은 dst의 head와 연결
        nxt[prv[cur]] = heads[m_dst];
        prv[heads[m_dst]] = prv[cur];
        heads[m_dst] = heads[m_src];
	}

    // src의 head는 cur
    heads[m_src] = cur;
    prv[cur] = -1; // head가 되었으므로 이전은 없음    

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