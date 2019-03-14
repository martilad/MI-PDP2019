//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_SOLUTION_H
#define MI_PDP2019_SOLUTION_H

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include "items.h"
#include "cord.h"
#include "constants.h"

/**
 * One found the solution of the map, the best for now.
 */
class solution {

public:
    int m;
    int n;
    int k;
    std::vector <std::vector<int>> ground;
    int price;
    int actPrice;
    int nL3;
    int nL4;
    int nEmptyBefore;
    int nEmptyAfter;
    /**
     * Print solution with 2D array and parameter of solution.
     */
    void printSolution();

    /**
     * Count and save coords of the empty positions in map.
     */
    std::vector <cord> findEmptyCoords();

    /**
    * Compute the price of solution.
    */
    void computePrice();

    /**
     * Compute actual price. No add possible fill field to price.
     */
    void computeActPrice();

    /**
    * Compute actual price. No add possible fill field to price.
    */
    void computeActPrice(int nL3, int nL4, int nEmptyBefore);

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
    cord nextFree(int x, int y) const;

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
    int addValueToMap(int id, int oldVal, int newVal, int x, int y);
};

#endif //MI_PDP2019_SOLUTION_H
