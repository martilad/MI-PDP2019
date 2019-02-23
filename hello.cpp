#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>

using namespace std;

enum possibility {
    empty, l31, l32, l33, l34, l35, l36, l37, l38, l41, l42, l43, l44, l45, l46, l47, l48
};
const int PRINT_NUMBERS = 2;

/**
 * Coordinate with position.
 */
struct cord {
    int x;
    int y;
};

/**
 * Structure to save one solution with map and needed parameters.
 */
class solution {

public:
    int m;
    int n;
    vector <vector<int>> ground;
    int price;
    int n_L3;
    int n_L4;
    int n_not;
    vector <cord> not_cords;

    void countEmptyCoords(){

    }
};

/**
 * Print solution with 2D array and parameter of solution.
 *
 * @param array array to print
 * @param m Number of columns
 * @param n number of rows
 */
void printSolution(solution solution, int m, int n) {
    cout << "Max price: " << solution.price << endl;
    cout << "Number of L3: " << solution.n_L3 << endl;
    cout << "Number of L4: " << solution.n_L4 << endl;
    cout << "Number of not fill: " << solution.n_not << endl;
    for (auto &cord : solution.not_cords) {
        cout << cord.x << " " << cord.y << endl;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m - 1; j++) {
            cout << setw(PRINT_NUMBERS) << setfill(' ')
                 << ((solution.ground[j][i] == -1) ? "Z" : (solution.ground[j][i] == 0) ? "-" : to_string(
                         solution.ground[j][i])) << " ";
        }
        cout << setw(PRINT_NUMBERS) << setfill(' ')
             << ((solution.ground[m-1][i] == -1) ? "Z" : (solution.ground[m-1][i] == 0) ? "-" : to_string(
                     solution.ground[m-1][i])) << endl;
    }
}


solution solveMap(vector <vector<int>> array) {
    solution best = solution();


    best.ground = array;
    return best;
}

int main() {
    // load arguments
    int m, n, k;
    cin >> m >> n >> k;

    vector <vector<int>> ground = vector < vector < int >> ();

    // init the ground
    for (int i = 0; i < m; i++) {
        vector<int> tmp = vector<int>();
        for (int j = 0; j < n; j++) {
            tmp.push_back(0);
        }
        ground.push_back(tmp);
    }

    // load the forbidden fields
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;
        if (x >= 0 && x < m && y >= 0 && y < n) {
            ground[x][y] = -1;
        }
    }
    printSolution(solveMap(ground), m, n);
}