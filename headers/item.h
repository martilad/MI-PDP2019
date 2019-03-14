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

    Item(solution sol, cord co, int cnt) {
        this->sol = sol;
        this->co = co;
        this->cnt = cnt;
    }

    solution sol;
    cord co;
    int cnt;
};

#endif //MI_PDP2019_STACKITEM_H
