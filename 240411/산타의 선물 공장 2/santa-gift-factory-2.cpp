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

    // src에 물건이 없다면 pass
    if (counts[m_src] == 0)
    {
        cout << counts[m_dst] << endl;
        return;
    }

    // dst에 물건이 없다면 그대로
    if (counts[m_dst] == 0)
    {
        heads[m_dst] = heads[m_src];
        tails[m_dst] = tails[m_src];
	}
    else
    {
        int orig_head = heads[m_dst];
        // m_dst의 head와 교체
        heads[m_dst] = heads[m_src];
        // m_src의 tail과 m_dst의 head 연결
        int src_tail = tails[m_src];
        nxt[src_tail] = orig_head;
        prv[orig_head] = src_tail;
    }

    // head, tail, count 갱신
    heads[m_src] = tails[m_src] = -1;

    counts[m_dst] += counts[m_src];
	counts[m_src] = 0;

    cout << counts[m_dst] << endl;
}

// 해당 벨트의 head 제거
int RemoveHead(int b_num)
{
    // 불가능하면 pass
    if (counts[b_num] == 0)
        return -1;

    // 노드가 1개라면
    // head, tail 전부 삭제 후 반환
    if (counts[b_num] == 1)
    {
        int id = heads[b_num];
        heads[b_num] = tails[b_num] = -1;
        counts[b_num] = 0;
        return id;
    }

    // head 변경
    int hid = heads[b_num];
    int next_head = nxt[hid];
    nxt[hid] = prv[next_head] = -1;
    counts[b_num]--;
    heads[b_num] = next_head;

    return hid;
}

// 해당 벨트에 head 추가
void PushHead(int b_num, int hid)
{
    if (hid == -1)
        return;

    if (counts[b_num] == 0)
    {
        heads[b_num] = tails[b_num] = hid;
		counts[b_num] = 1;
    }
    else
    {
        int orig_head = heads[b_num];
        nxt[hid] = orig_head;
        prv[orig_head] = hid;
        heads[b_num] = hid;
        counts[b_num]++;
    }
}

void Swap()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    int src_head = RemoveHead(m_src);
    int dst_head = RemoveHead(m_dst);
    PushHead(m_src, dst_head);
    PushHead(m_dst, src_head);

    cout << counts[m_dst] << endl;
}

void Divide()
{
    int m_src, m_dst;
    cin >> m_src >> m_dst;

    // 순서대로 src에서 박스들을 뺀다.
    int cnt = counts[m_src];
    vector<int> boxes;

    for (int i = 0; i < cnt / 2; i++)
    {
		int box = RemoveHead(m_src);
		boxes.push_back(box);
	}

    reverse(boxes.begin(), boxes.end());

    for (int i = 0; i < boxes.size(); i++)
    {
		PushHead(m_dst, boxes[i]);
	}

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
            Swap();
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