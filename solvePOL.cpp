#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <stack>

using namespace std;

enum possibility {
    no = 0,
    l31 = 1,
    l32 = 2,
    l33 = 3,
    l34 = 4,
    l35 = 5,
    l36 = 6,
    l37 = 7,
    l38 = 8,
    l41 = 9,
    l42 = 10,
    l43 = 11,
    l44 = 12,
    l45 = 13,
    l46 = 14,
    l47 = 15,
    l48 = 16,
    empty = 17
};
const int PRINT_NUMBERS = 2;

/**
 * Coordinate with position.
 */
struct cord {

    cord(int x, int y) {
        this->x = x;
        this->y = y;
    }

    cord (const cord& tmp){
        this->x = tmp.x;
        this->y = tmp.y;
    }

    cord(){

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

struct stackItem {

    stackItem(cord coordinates, int id, int cnt){
        this->coordinates = coordinates;
        this->id = id;
        this->cnt = cnt;
    }

    cord coordinates;
    int id;
    int cnt;
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
        for (int i = 0; i < this->m; i++)
            for (int j = 0; j < this->n; j++)
                if (this->ground[i][j] == 0) {
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
    static int countEmptyCoords(vector <vector<int>> *array) {
        int cnt = 0;
        for (unsigned int i = 0; i < (*array).size(); i++)
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
    static int computePrice(int L3, int L4, int n_empty) {
        return 2 * L3 + 3 * L4 - 6 * n_empty;
    }

    /**
     * Count the maximal price for the "number" of unsolved squares.
     *
     * @param number number of unsolved squares
     * @return returns the maximal price for the "number" of unsolved squares
     */
    static int eval_pol(int number) {
        int mod = number % 5;
        if (mod == 0) return (number / 5) * 3;
        else if (mod == 1) return ((number - 16) / 5) * 3 + 4 * 2;
        else if (mod == 2) return ((number - 12) / 5) * 3 + 3 * 2;
        else if (mod == 3) return ((number - 8) / 5) * 3 + 2 * 2;
        else return ((number - 4) / 5) * 3 + 2;
    }

    cord nextFree(vector <vector<int>> *array, int x, int y){
        do {
            x = x + 1;
            y = y + x/this->n;
            x = x % this->n;
            if (y >= this->m){
                x = -1;
                y = -1;
            }
        } while (x != -1 || (y == -1) ? false : (*array)[y][x] != 0);
        return cord(x, y);
    }

    void solveMap() {
        stack<stackItem> iterationStack;
        int nEmpty = this->countEmptyCoords(&this->ground);
        int L3 = 0;
        int L4 = 0;
        int cnt = 1;
        iterationStack.push(stackItem(this->nextFree(&this->ground, 0, 0), no, cnt));
        while (iterationStack.size() > 0){
            stackItem tmp = iterationStack.top();
            iterationStack.pop();
            //cout << tmp.coordinates.x << " " << tmp.coordinates.y << " " << tmp.id << " " << tmp.cnt << endl;
            if (tmp.id == empty || tmp.coordinates.x == -1 || tmp.coordinates.y == -1){
                continue;
            }
            if (tmp.id > no){
                //cout << "need remove" << endl;
            }
            if (tmp.coordinates.x == 4 && tmp.coordinates.y == 12){
                cout << tmp.id << endl;
            }
            //cout << "need push next"  << empty << endl;
            iterationStack.push(stackItem(cord(tmp.coordinates.x, tmp.coordinates.y), ++tmp.id, tmp.cnt++));
            cord next = this->nextFree(&this->ground, tmp.coordinates.x, tmp.coordinates.y);
            if (next.x != -1 && next.y != -1){
                iterationStack.push(stackItem(this->nextFree(&this->ground, tmp.coordinates.x, tmp.coordinates.y), no, cnt));
            }

            // price check and save the best
        }
        cout << "Solve the problem" << L3 << " " << L4<< " " << nEmpty << endl;
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
        /*for (auto &cord : this->not_cords) {
            cout << cord.x << " " << cord.y << endl;
        }*/

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << setw(PRINT_NUMBERS) << setfill(' ')
                     << ((this->ground[i][j] == -1) ? "Z" : (this->ground[i][j] == 0) ? "-" : to_string(
                             this->ground[i][j])) << " ";
            }
            cout << setw(PRINT_NUMBERS) << setfill(' ')
                 << ((this->ground[i][n - 1] == -1) ? "Z" : (this->ground[i][n - 1] == 0) ? "-" : to_string(
                         this->ground[i][n - 1])) << endl;
        }
    }
};

int main() {
    POL *problem = new POL();
    problem->loadProblem();
    problem->solveMap();
    problem->findEmptyCoords();
    problem->printSolution();
    cout << problem->eval_pol(9 * 9);
    delete problem;
}
