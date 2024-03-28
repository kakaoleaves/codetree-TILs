#include <iostream>

using namespace std;

bool isPrime(int n)
{
    if (n < 2) return false;

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    long long sum = 0;
    int min_prime = 0;
    cin >> a >> b;
    for (int i = a; i <= b; i++)
    {
        if (isPrime(i)) {
            if (min_prime == 0) min_prime = i;
            sum += i;
        }
    }

    cout << sum << ' ' << min_prime << '\n';

    return 0;
}