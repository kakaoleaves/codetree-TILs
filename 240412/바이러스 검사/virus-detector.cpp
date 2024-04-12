#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 1 <= n <= 1'000'000 ; 시험장의 개수
int n;
// 1 <= a[i] <= 1'000'000; a[i] : i번째 시험장의 응시자 수
vector<int> a;
// 1 <= b, c <= 1'000'000; 감독관이 감시할 수 있는 응시자 수
int b, c;
long long ans;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n; 
	a.resize(n);

	for (int i = 0; i < n; i++)
		cin >> a[i];

	cin >> b >> c;

	for (int i = 0; i < n; i++) {
		a[i] -= b;
		ans += 1;
		if (a[i] > 0) {
			ans += a[i] / c;
			if (a[i] % c > 0)
				ans += 1;
		}
	}

	cout << ans << '\n';

	return 0;
}