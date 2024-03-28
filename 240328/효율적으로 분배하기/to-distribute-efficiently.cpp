#include <iostream>

using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int ans = -1;

    cin >> n;

    for (int i = (n / 5); i >= 0; i--)
    {
        int rest = n - 5 * i;
        if (rest % 3 == 0)
        {
            ans = rest / 3 + i;
            break;
        }        
    }

    cout << ans << '\n';    
    
    return 0;
}