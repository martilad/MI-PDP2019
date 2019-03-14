//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_STACKITEM_H
#define MI_PDP2019_STACKITEM_H

#include "cord.h"
#include "solution.h"

/**
 * Item to save to the iteration stack.
 */
struct Item {

    Item(solution sol, cord coordinates, int id, int cnt) {
        this->sol = sol;
        this->coordinates = coordinates;
        this->id = id;
        this->cnt = cnt;
    }

    solution sol;
    cord coordinates;
    int id;
    int cnt;
};

#endif //MI_PDP2019_STACKITEM_H