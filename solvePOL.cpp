#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <ctime>

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

// items defined by relative coordinates
const vector <cord> L31 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0)};
const vector <cord> L32 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2)};
const vector <cord> L33 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(1, 2)};
const vector <cord> L34 = {cord(0, 0), cord(-1, 1), cord(-2, 1), cord(0, 1)};
const vector <cord> L35 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2)};
const vector <cord> L36 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(2, 1)};
const vector <cord> L37 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1)};
const vector <cord> L38 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(-1, 2)};

const vector <cord> L41 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0), cord(3, 0)};
const vector <cord> L42 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2), cord(1, 3)};
const vector <cord> L43 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(0, 3), cord(1, 3)};
const vector <cord> L44 = {cord(0, 0), cord(0, 1), cord(-1, 1), cord(-2, 1), cord(-3, 1)};
const vector <cord> L45 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2), cord(0, 3)};
const vector <cord> L46 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(3, 0), cord(3, 1)};
const vector <cord> L47 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1), cord(3, 1)};
const vector <cord> L48 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(0, 3), cord(-1, 3)};

const vector <vector<cord>> items = {{}, L41, L42, L43, L44, L45, L46, L47, L48, L31, L32, L33, L34, L35, L36, L37,
                                     L38};


/**
 * Count the maximal price for the "number" of unsolved squares.
 *
 * @param number number of unsolved squares
 * @return returns the maximal price for the "number" of unsolved squares
 */
static int eval_pol(int number) {
    if (number == 1) return -6;
    if (number == 2) return -12;
    if (number == 3) return -18;
    if (number == 6) return -3;
    if (number == 7) return -9;
    if (number == 11) return 0;
    int mod = number % 5;
    if (mod == 0) return (number / 5) * 3;
    else if (mod == 1) return ((number - 16) / 5) * 3 + 4 * 2;
    else if (mod == 2) return ((number - 12) / 5) * 3 + 3 * 2;
    else if (mod == 3) return ((number - 8) / 5) * 3 + 2 * 2;
    else return ((number - 4) / 5) * 3 + 2;
}

/**
 * One found the solution of the map, the best for now.
 */
struct solution {

    /**
     * Print solution with 2D array and parameter of solution.
     */
    void printSolution() {
        vector <cord> notCords;
        notCords = this->findEmptyCoords();
        this->computePrice();
        cout << "Max price: " << this->price << endl;
        cout << "Number of L3: " << this->nL3 << endl;
        cout << "Number of L4: " << this->nL4 << endl;
        cout << "Number of not fill: " << this->nEmptyBefore + this->nEmptyAfter << endl;
        for (auto &cord : notCords) {
            cout << cord.x << " " << cord.y << endl;
        }

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
    vector <cord> findEmptyCoords() {
        vector <cord> notCords = vector<cord>();
        int cnt = 0;
        for (int i = 0; i < this->m; i++)
            for (int j = 0; j < this->n; j++)
                if (this->ground[i][j] == 0) {
                    notCords.push_back(cord(j, i));
                    cnt++;
                }
        this->nEmptyBefore = cnt;
        this->nEmptyAfter = 0;
        return notCords;
    }

    /**
    * Compute the price of solution.
    */
    void computePrice() {
        this->price = 2 * this->nL3 + 3 * this->nL4 - 6 * (this->nEmptyAfter + this->nEmptyBefore);
    }

    /**
     * Compute actual price. No add possible fill field to price.
     */
    void computeActPrice() {
        this->actPrice = 2 * this->nL3 + 3 * this->nL4 - 6 * this->nEmptyBefore;
    }

    /**
    * Compute actual price. No add possible fill field to price.
    */
    void computeActPrice(int nL3, int nL4, int nEmptyBefore) {
        this->actPrice = 2 * nL3 + 3 * nL4 - 6 * nEmptyBefore;
    }

    /**
     * Found next free field in array.
     *
     * @param array where to find the field
     * @param m number of rows
     * @param n number of columns
     * @param x x cord of from position
     * @param y y cord of from position
     * @return coordinates of free position
     */
    cord nextFree(int x, int y) const {
        do {
            x = x + 1;
            y = y + x / this->n;
            x = x % this->n;
            if (y >= this->m) {
                x = -1;
                y = -1;
                break;
            }
        } while (this->ground[y][x] != 0);
        return cord(x, y);
    }

    /**
     * Place one item on map. Item defined by cords and the fields of before have to have same value set, In the vector is set the new value.
     *
     * @param array ground to fill
     * @param m number of rows
     * @param n number of columns
     * @param id id of item to place
     * @param oldVal old value in map
     * @param newVal new value in map
     * @param x x cord to start place item
     * @param x y cord to start place item
     * @return 1 success || -1 not success
     */
    int addValueToMap(int id, int oldVal, int newVal, int x, int y) {
        int tmpX, tmpY;
        if (id == empty) return 1;
        vector <cord> v = items[id];
        for (unsigned int i = 0; i < v.size(); i++) {
            tmpX = x + v[i].x;
            tmpY = y + v[i].y;
            if (tmpX >= this->n || tmpY >= this->m || tmpX < 0 || tmpY < 0) return -1;
            if (this->ground[tmpY][tmpX] != oldVal) return -1;
        }
        for (unsigned int i = 0; i < v.size(); i++) {
            this->ground[y + v[i].y][x + v[i].x] = newVal;
        }
        return 1;
    }

    int m;
    int n;
    int k;
    vector <vector<int>> ground;
    int price;
    int actPrice;
    int nL3;
    int nL4;
    int nEmptyBefore;
    int nEmptyAfter;
};

/**
 * Item to save to the iteration stack.
 */
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
 * One solution class.
 */
class POL {
    solution workSolution;
    solution bestSolution;
    int m;
    int n;
    bool load;
    int possibleBest;
public:

    /**
     * Getter for the best solution in map.
     * @return the best reach solution
     */
    solution getBest() {
        return this->bestSolution;
    }

    void solveMapRecursion(){
        this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
        this->workSolution.nEmptyBefore = 0;
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        // possible the best solution on map
        this->possibleBest = eval_pol(this->m * this->n - this->workSolution.k);
        // init best solution
        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();
    }
    //TODO: solve one recursion with copy the whole solution.
    void solveRecursion1(solution workSolution, cord cord, int cnt){
        for (int id = l41; id < empty+1;id++){
            // pridat pod cnt do mapy
            // pokud se povede pridat tak zkontrolovat jestli je lepsi, nebo jestli neni uplne nejlepsi

            // find free from cord
            // free je mimo -> return
            // a zavolat znova s touto hodnotou

            //po navratu odebrat aby se mohla zkusit nova hodnota
        }
    }

    //TODO: second recursion only with cord pass, to much harder to do parallel
    void solveRecursion2(cord cord, int cnt){
        for (int id = l41; id < empty+1;id++){
            // pridat pod cnt do mapy
            // pokud se povede pridat tak zkontrolovat jestli je lepsi, nebo jestli neni uplne nejlepsi

            // find free from cord
            // free je mimo -> return
            // a zavolat znova s touto hodnotou

            //po navratu odebrat aby se mohla zkusit nova hodnota
        }
    }

    /**
     * Solve the map with items in possibility.
     */
    void solveMap() {
        // iteration stack for operations
        stack <stackItem> iterationStack;
        this->workSolution.nEmptyAfter = this->n * this->m - this->workSolution.k;
        this->workSolution.nEmptyBefore = 0;
        this->workSolution.nL3 = 0;
        this->workSolution.nL4 = 0;

        // possible the best solution on map
        this->possibleBest = eval_pol(this->m * this->n - this->workSolution.k);
        // init best solution
        this->bestSolution = this->workSolution;
        this->bestSolution.computePrice();

        // add first possible place for add some item
        iterationStack.push(stackItem(this->workSolution.nextFree(-1, 0), no, 1));

        while (iterationStack.size() > 0) {
            stackItem tmp = iterationStack.top();
            iterationStack.pop();
            // if empty go back to next (back) position with some. (after place empty there is no next possibility there)
            if (tmp.id == empty) {
                this->workSolution.nEmptyAfter += 1;
                this->workSolution.nEmptyBefore -= 1;
                continue;
            }

            // if there some item remove it, recount counts of items
            if (tmp.id > no) {
                this->workSolution.addValueToMap(tmp.id, tmp.cnt, 0, tmp.coordinates.x, tmp.coordinates.y);
                if (tmp.id >= l41 && tmp.id <= l48) {
                    this->workSolution.nL4 -= 1;
                    this->workSolution.nEmptyAfter += 5;
                }
                if (tmp.id >= l31 && tmp.id <= l38) {
                    this->workSolution.nL3 -= 1;
                    this->workSolution.nEmptyAfter += 4;
                }
            }

            // add some next item on position, add item count and empty places count
            while (1) {
                int ret = this->workSolution.addValueToMap(++tmp.id, 0, tmp.cnt, tmp.coordinates.x, tmp.coordinates.y);
                if (tmp.id == empty || ret != -1) {
                    if (tmp.id >= l41 && tmp.id <= l48) {
                        this->workSolution.nL4 += 1;
                        this->workSolution.nEmptyAfter -= 5;
                    }
                    if (tmp.id >= l31 && tmp.id <= l38) {
                        this->workSolution.nL3 += 1;
                        this->workSolution.nEmptyAfter -= 4;
                    }
                    if (tmp.id == empty) {
                        this->workSolution.nEmptyAfter -= 1;
                        this->workSolution.nEmptyBefore += 1;
                    }
                    break;
                }
            }

            // add placed item to iteration stack
            iterationStack.push(stackItem(cord(tmp.coordinates.x, tmp.coordinates.y), tmp.id, tmp.cnt));

            // check act price
            this->workSolution.computePrice();

            // if reach the maximum can end
            if (this->workSolution.price == this->possibleBest) {
                this->bestSolution = this->workSolution;
                return;
            }
            // if act solution is better than best solution -> replace
            if (this->workSolution.price > this->bestSolution.price) {
                this->bestSolution = this->workSolution;
            }
            // check if act price + possible price can beat the max
            this->workSolution.computeActPrice();
            if (this->workSolution.actPrice + eval_pol(this->workSolution.nEmptyAfter) <= this->bestSolution.price) {
                continue;
            }

            // cant put some in the last line
            if (tmp.coordinates.y != this->m - 1) {
                if (tmp.id < empty) tmp.cnt++;
                cord next = this->workSolution.nextFree(tmp.coordinates.x, tmp.coordinates.y);

                if (next.x != -1 && next.y != -1) {
                    // add new free field to check possibility of this field
                    iterationStack.push(stackItem(next, no, tmp.cnt));
                }
            }
        }
    }

    /**
     * Load the problem from standard input.
     */
    void loadProblem(std::istream& in) {
        // load arguments
        int k;
        in >> this->m >> this->n >> k;
        this->workSolution.m = this->m;
        this->workSolution.n = this->n;
        this->workSolution.k = k;
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
            in >> x >> y;
            if (x >= 0 && x < n && y >= 0 && y < m) {
                this->workSolution.ground[y][x] = -1;
            }
        }
        this->load = true;
    }

    /**
     * Check for successfully load the problem.
     * @return true if load the problem
     */
    bool isLoad(){
        return this->load;
    }
};

int main(int argc, char* argv[]) {
    char *myFile = nullptr;
    bool stdIn = false;
    // load the arguments
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && (i + 1 <= argc))  {
            myFile = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cout << "Use -f <file> for the load from file or -i switch if you want to standard input." << endl;
            exit(0);
        } else if (strcmp(argv[i], "-i") == 0) {
            stdIn = true;
        }else {
            cout << "Not enough or invalid arguments, please try again." << endl;
            exit(1);
        }
    }
    POL *problem = new POL();

    // load the problem
    if (myFile){
        ifstream file;
        file.open(myFile);
        if (file.is_open()){
            problem->loadProblem(file);
            file.close();
            stdIn = false;
        }
    }
    if (stdIn) problem->loadProblem(cin);

    if (problem->isLoad()){
        clock_t begin = clock();
        problem->solveMap();
        clock_t end = clock();
        solution best = problem->getBest();
        best.printSolution();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << "Solution time: "<< elapsed_secs << " s." << endl;
        delete problem;
    }else{
        cout << "Problem not load." << endl;
    }
}
