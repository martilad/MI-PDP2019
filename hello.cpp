#include <iostream>
#include <vector>

using namespace std;

int main(){
	int m, n, k;
	cin >> m >> n >> k;

	for (int i = 0; i < k; i++){
		int x, y;
		cin >> x >> y;
		cout << x << "--" << y << endl;
	}
	
	cout << m << endl;
	cout << n << endl;
	cout << k << endl;

}