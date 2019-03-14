//
// Created by martilad on 3/14/19.
//
#include "../headers/solution.h"

/**
 * Print solution with 2D array and parameter of solution.
 */
void solution::printSolution() {

    std::vector <cord> notCords;
    notCords = this->findEmptyCoords();
    this->computePrice();
    std::cout << "Max price: " << this->price << std::endl;
    std::cout << "Number of L3: " << this->nL3 << std::endl;
    std::cout << "Number of L4: " << this->nL4 << std::endl;
    std::cout << "Number of not fill: " << this->nEmptyBefore + this->nEmptyAfter << std::endl;
    for (auto &cord : notCords) {
        std::cout << cord.x << " " << cord.y << std::endl;
    }

    for (int i = 0; i < this->m; i++) {
        for (int j = 0; j < this->n - 1; j++) {
            std::cout << std::setw(PRINT_NUMBERS) << std::setfill(' ')
                      << ((this->ground[i][j] == -1) ? "Z" : (this->ground[i][j] == 0) ? "-" : std::to_string(
                              this->ground[i][j])) << " ";
        }
        std::cout << std::setw(PRINT_NUMBERS) << std::setfill(' ')
                  << ((this->ground[i][this->n - 1] == -1) ? "Z" : (this->ground[i][this->n - 1] == 0) ? "-"
                                                                                                       : std::to_string(
                                  this->ground[i][this->n - 1])) << std::endl;
    }
}

/**
 * Count and save coords of the empty positions in map.
 */
std::vector <cord> solution::findEmptyCoords() {
    std::vector <cord> notCords = std::vector<cord>();
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
void solution::computePrice() {
    this->price = 2 * this->nL3 + 3 * this->nL4 - 6 * (this->nEmptyAfter + this->nEmptyBefore);
}

/**
 * Compute actual price. No add possible fill field to price.
 */
void solution::computeActPrice() {
    this->actPrice = 2 * this->nL3 + 3 * this->nL4 - 6 * this->nEmptyBefore;
}

/**
* Compute actual price. No add possible fill field to price.
*/
void solution::computeActPrice(int nL3, int nL4, int nEmptyBefore) {
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
cord solution::nextFree(int x, int y) const {
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
int solution::addValueToMap(int id, int oldVal, int newVal, int x, int y) {
    int tmpX, tmpY;
    if (id == empty) return id;
    std::vector <cord> v = items[id];
    for (unsigned int i = 0; i < v.size(); i++) {
        tmpX = x + v[i].x;
        tmpY = y + v[i].y;
        if (tmpX >= this->n || tmpY >= this->m || tmpX < 0 || tmpY < 0) return -1;
        if (this->ground[tmpY][tmpX] != oldVal) return -1;
    }
    for (unsigned int i = 0; i < v.size(); i++) {
        this->ground[y + v[i].y][x + v[i].x] = newVal;
    }
    return id;
}

