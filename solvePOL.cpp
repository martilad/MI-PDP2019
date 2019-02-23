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

    cord(int x, int y){
        this->x = x;
        this->y = y;
    }

    int x;
    int y;
};

/**
 * One find solution of map, the best for now.
 */
struct solution {
    vector <vector<int>> ground;
    int price;
    int n_L3;
    int n_L4;
    int n_empty;
};

/**
 * Structure to save one solution with map and needed parameters.
 */
class POL {
    int m;
    int n;
    vector <vector<int>> ground;
    int price;
    int n_L3;
    int n_L4;
    int n_not;
    vector <cord> not_cords;

public:

    /**
     * Count and save coords of the empty positions in map.
     */
    void findEmptyCoords() {
        int cnt = 0;
        for (int i  = 0; i < this->m; i++)
            for (int j = 0; j < this->n; j++)
                if (this->ground[i][j] == 0){
                    this->not_cords.push_back(cord(j, i));
                    cnt++;
                }
        this->n_not = cnt;
    }

    /**
     * Count number of empty fields.
     * @param array array where find the empty -> value == 0
     * @return number of emtpy fields
     */
    static int countEmptyCoords(vector <vector<int>> * array) {
        int cnt = 0;
        for (unsigned int i  = 0; i < (*array).size(); i++)
            for (unsigned int j = 0; j < (*array)[i].size(); j++)
                if ((*array)[i][j] == 0)
                    cnt++;
        return cnt;
    }

    /**
     * Compute the price of solution.
     * @param L3 number of L3
     * @param L4 number of L4
     * @param n_empty number of empty field
     * @return price of the solution
     */
    static int computePrice(int L3, int L4, int n_empty){
        return 2*L3+3*L4-6*n_empty;
    }

    /**
     * Count the maximal price for the "number" of unsolved squares.
     *
     * @param number number of unsolved squares
     * @return returns the maximal price for the "number" of unsolved squares
     */
    static int eval_pol(int number){
        int mod=number%5;
        if (mod==0) return (number/5)*3;
        else if (mod==1) return ((number-16)/5)*3+4*2;
        else if (mod==2) return ((number-12)/5)*3+3*2;
        else if (mod==3) return ((number-8)/5)*3+2*2;
        else return ((number-4)/5)*3+2;
    }

    void solveMap() {
        cout << "Solve the problem" << endl;
    }

    /**
     * Load the problem from standard input.
     */
    void loadProblem() {
        // load arguments
        int k;
        cin >> this->m >> this->n >> k;

        this->ground = vector < vector < int >> ();

        // init the ground
        for (int i = 0; i < m; i++) {
            vector<int> tmp = vector<int>();
            for (int j = 0; j < n; j++) {
                tmp.push_back(0);
            }
            this->ground.push_back(tmp);
        }

        // load the forbidden fields
        for (int i = 0; i < k; i++) {
            int x, y;
            cin >> x >> y;
            if (x >= 0 && x < n && y >= 0 && y < m) {
                this->ground[y][x] = -1;
            }
        }
    }

    /**
     * Print solution with 2D array and parameter of solution.
     */
    void printSolution() {
        cout << "Max price: " << this->price << endl;
        cout << "Number of L3: " << this->n_L3 << endl;
        cout << "Number of L4: " << this->n_L4 << endl;
        cout << "Number of not fill: " << this->n_not << endl;
        for (auto &cord : this->not_cords) {
            cout << cord.x << " " << cord.y << endl;
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << setw(PRINT_NUMBERS) << setfill(' ')
                     << ((this->ground[i][j] == -1) ? "Z" : (this->ground[i][j] == 0) ? "-" : to_string(
                             this->ground[i][j])) << " ";
            }
            cout << setw(PRINT_NUMBERS) << setfill(' ')
                 << ((this->ground[i][n-1] == -1) ? "Z" : (this->ground[i][n-1] == 0) ? "-" : to_string(
                         this->ground[i][n-1])) << endl;
        }
    }
};

int main() {
    POL *problem = new POL();
    problem->loadProblem();
    problem->solveMap();
    problem->findEmptyCoords();
    problem->printSolution();
    cout << problem->eval_pol(9*9);
    delete problem;
}
