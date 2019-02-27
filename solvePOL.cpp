#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <stack>
#include <queue>

using namespace std;

const int PRINT_NUMBERS = 2;

enum possibility {
    no = 0,
    l41 = 1,
    l42 = 2,
    l43 = 3,
    l44 = 4,
    l45 = 5,
    l46 = 6,
    l47 = 7,
    l48 = 8,
    l31 = 9,
    l32 = 10,
    l33 = 11,
    l34 = 12,
    l35 = 13,
    l36 = 14,
    l37 = 15,
    l38 = 16,
    empty = 17
};

/**
 * Coordinate with position.
 */
struct cord {

    cord(int x, int y) {
        this->x = x;
        this->y = y;
    }

    cord(const cord &tmp) {
        this->x = tmp.x;
        this->y = tmp.y;
    }

    cord() {

    }

    int x;
    int y;
};

const vector <cord> L31 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0)};
const vector <cord> L32 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2)};
const vector <cord> L33 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(1, 2)};
const vector <cord> L34 = {cord(2, -1), cord(0, 0), cord(1, 0), cord(2, 0)};
const vector <cord> L35 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2)};
const vector <cord> L36 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(2, 1)};
const vector <cord> L37 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1)};
const vector <cord> L38 = {cord(1, -2), cord(1, -1), cord(1, 0), cord(0, 0)};

const vector <cord> L41 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0), cord(3, 0)};
const vector <cord> L42 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2), cord(1, 3)};
const vector <cord> L43 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(0, 3), cord(1, 3)};
const vector <cord> L44 = {cord(3, -1), cord(0, 0), cord(1, 0), cord(2, 0), cord(3, 0)};
const vector <cord> L45 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2), cord(0, 3)};
const vector <cord> L46 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(3, 0), cord(3, 1)};
const vector <cord> L47 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1), cord(3, 1)};
const vector <cord> L48 = {cord(1, -3), cord(1, -2), cord(1, -1), cord(1, 0), cord(0, 0)};

const vector<vector<cord>> items = {{}, L41, L42, L43, L44, L45, L46, L47, L48, L31, L32, L33, L34, L35, L36, L37, L38};





static cord nextFree(vector <vector<int>> *array, int m, int n, int x, int y) {
    do {
        x = x + 1;
        y = y + x / n;
        x = x % n;
        if (y >= m) {
            x = -1;
            y = -1;
            break;
        }
    } while ((*array)[y][x] != 0);
    return cord(x, y);
}

static int addValueToMap(vector <vector<int>> *array, int m, int n, int id, int oldVal, int newVal, int x, int y) {
    int tmpX, tmpY;
    if (id == empty) return 1;
    vector <cord> v = items[id];
    for (unsigned int i = 0; i < v.size(); i++) {
        tmpX = x + v[i].x;
        tmpY = y + v[i].y;
        if (tmpX >= n || tmpY >= m || tmpX < 0 || tmpY < 0) return -1;
        if ((*array)[tmpY][tmpX] != oldVal) return -1;
    }
    for (unsigned int i = 0; i < v.size(); i++) {
        (*array)[y + v[i].y][x + v[i].x] = newVal;
    }
    return 1;
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

/**
 * One find solution of map, the best for now.
 */
struct solution {

    /**
     * Print solution with 2D array and parameter of solution.
     */
    void printSolution() {
        this->computePrice();
        cout << "Max price: " << this->price << endl;
        cout << "Number of L3: " << this->nL3 << endl;
        cout << "Number of L4: " << this->nL4 << endl;
        cout << "Number of not fill: " << this->nEmpty << endl;
        /*for (auto &cord : this->not_cords) {
            cout << cord.x << " " << cord.y << endl;
        }*/

        for (int i = 0; i < this->m; i++) {
            for (int j = 0; j < this->n - 1; j++) {
                cout << setw(PRINT_NUMBERS) << setfill(' ')
                     << ((this->ground[i][j] == -1) ? "Z" : (this->ground[i][j] == 0) ? "-" : to_string(
                             this->ground[i][j])) << " ";
            }
            cout << setw(PRINT_NUMBERS) << setfill(' ')
                 << ((this->ground[i][this->n - 1] == -1) ? "Z" : (this->ground[i][this->n - 1] == 0) ? "-" : to_string(
                         this->ground[i][this->n - 1])) << endl;
        }
    }

    /**
     * Count and save coords of the empty positions in map.
     */
    void findEmptyCoords() {
        notCords = vector<cord>();
        int cnt = 0;
        for (int i = 0; i < this->m; i++)
            for (int j = 0; j < this->n; j++)
                if (this->ground[i][j] == 0) {
                    this->notCords.push_back(cord(j, i));
                    cnt++;
                }
        this->nEmpty = cnt;
    }

    /**
    * Compute the price of solution.
    */
    void computePrice() {
        this->countEmptyCoords();
        this->price =  2 * this->nL3 + 3 * this->nL4 - 6 * this->nEmpty;
    }

    /**
     * Count number of empty fields.
     */
     void countEmptyCoords() {
        int cnt = 0;
        for (unsigned int i = 0; i < this->ground.size(); i++)
            for (unsigned int j = 0; j < this->ground[i].size(); j++)
                if (this->ground[i][j] == 0)
                    cnt++;
        this->nEmpty = cnt;
    }

    int m;
    int n;
    vector <vector<int>> ground;
    int price;
    int nL3;
    int nL4;
    int nEmpty;
    vector <cord> notCords;
};




struct stackItem {

    stackItem(cord coordinates, int id, int cnt) {
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
    solution workSolution;
    solution bestSolution;
    int m;
    int n;
public:

    solution getBest(){
        return this->bestSolution;
    }

    void solveMap() {
        // iteration stack for operations
        int cnt = 0;
        stack <stackItem> iterationStack;
        this->workSolution.countEmptyCoords();
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();

        // add first possible place for add some item
        iterationStack.push(stackItem(nextFree(&this->workSolution.ground, this->m, this->n, 0, 0), no, 1));
        while (iterationStack.size() > 0) {
            stackItem tmp = iterationStack.top();
            iterationStack.pop();

            // if empty go back to next (back) position with some, after empty it is no possibility there
            if (tmp.id == empty || tmp.coordinates.x == -1 || tmp.coordinates.y == -1) {
                continue;
            }
            // if there some item remove it
            if (tmp.id > no) {
                addValueToMap(&this->workSolution.ground, this->m, this->n, tmp.id, tmp.cnt, 0, tmp.coordinates.x, tmp.coordinates.y);
                if (tmp.id >= l41 && tmp.id <= l48)this->workSolution.nL4 -= 1;
                if (tmp.id >= l31 && tmp.id <= l38)this->workSolution.nL3 -= 1;

                //TODO: if there is item remove it
            }
            // add some next item on position
            while (1) {
                int ret = addValueToMap(&this->workSolution.ground, this->m, this->n, ++tmp.id, 0, tmp.cnt, tmp.coordinates.x, tmp.coordinates.y);
                if (tmp.id == empty || ret != -1){
                    if (tmp.id >= l41 && tmp.id <= l48)this->workSolution.nL4 += 1;
                    if (tmp.id >= l31 && tmp.id <= l38)this->workSolution.nL3 += 1;
                    //TODO: some add count the filds
                    break;
                }

            }

            // add placed item to iteration stack
            iterationStack.push(stackItem(cord(tmp.coordinates.x, tmp.coordinates.y), tmp.id, tmp.cnt));
            if (tmp.id < empty) tmp.cnt++;
            cord next = nextFree(&this->workSolution.ground, this->m, this->n, tmp.coordinates.x, tmp.coordinates.y);

            // add new free field to check possibility
            if (next.x != -1 && next.y != -1) {
                iterationStack.push(
                        stackItem(nextFree(&this->workSolution.ground, this->m, this->n, tmp.coordinates.x, tmp.coordinates.y), no, tmp.cnt));
            }

            // TODO: price check and save the best
            /*cout  << " -------------BEST------------------- " << endl;
            this->bestSolution.printSolution();
            cout  << " -------------------------------- " << endl;*/

            this->workSolution.computePrice();
            if (this->workSolution.price > this->bestSolution.price){
                this->bestSolution = this->workSolution;
            }

            //this->workSolution.printSolution();


            // TODO: kill branch which can not beat the best. branch and bound.

        }
    }

    /**
     * Load the problem from standard input.
     */
    void loadProblem() {
        // load arguments
        int k;
        cin >> this->m >> this->n >> k;
        this->workSolution.m = this->m;
        this->workSolution.n = this->n;
        this->workSolution.ground = vector < vector < int >> ();

        // init the ground
        for (int i = 0; i < m; i++) {
            vector<int> tmp = vector<int>();
            for (int j = 0; j < n; j++) {
                tmp.push_back(0);
            }
            this->workSolution.ground.push_back(tmp);
        }

        // load the forbidden fields
        for (int i = 0; i < k; i++) {
            int x, y;
            cin >> x >> y;
            if (x >= 0 && x < n && y >= 0 && y < m) {
                this->workSolution.ground[y][x] = -1;
            }
        }
    }
};

int main() {
    POL *problem = new POL();
    problem->loadProblem();
    problem->solveMap();
    solution best = problem->getBest();
    best.findEmptyCoords();
    best.printSolution();
    delete problem;
}
