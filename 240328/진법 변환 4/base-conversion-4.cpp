#include <iostream>
#include <string>

using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string str;
    int ans = 0;
    cin >> str;

    int sz = str.size();
    for (int i = 0; i < sz; i++)
    {
        ans += ((str[i] - '0') << (sz - i - 1));
    }

    cout << ans << '\n';

    return 0;
}