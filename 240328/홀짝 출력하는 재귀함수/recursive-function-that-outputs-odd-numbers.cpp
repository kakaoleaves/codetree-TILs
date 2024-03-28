#include <iostream>

using namespace std;

void Recursion(int n)
{
    if (n == 1 || n == 2)
    {
        cout << n << ' ';
        return;
    }

    Recursion(n - 2);

    cout << n << ' ';
}

int main() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;

    cin >> n;

    Recursion(n);

    return 0;
}