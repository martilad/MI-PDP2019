#include <iostream>
#include <vector>

using namespace std;

void print(vector<vector<int>> array, int m, int n){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            cout << array[i][j];
        }
        cout << endl;
    }
}

struct cord{
    int x;
    int y;
};

struct solution{
    vector<vector<int>> ground;
    int price;
    int n_L3;
    int n_L4;
    int n_not;
    vector<cord> not_cords;
};


int main(){
	int m, n, k;
	cin >> m >> n >> k;

	vector<vector<int>> ground = vector<vector<int>>();

	for (int i = 0; i < m; i++){
	    vector<int> tmp = vector<int>();
	    for (int j = 0; j < n; j++){
	        tmp.push_back(0);
	    }
        ground.push_back(tmp);
	}

    for (int i = 0; i < k; i++){
        int x, y;
        cin >> x >> y;
        if (x >= 0 && x < m && y >=0 && y < n){
            ground[x][y] = 1;
        }
    }

	print(ground, m, n);

	cout << m << endl;
	cout << n << endl;
	cout << k << endl;
}