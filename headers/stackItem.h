//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_STACKITEM_H
#define MI_PDP2019_STACKITEM_H

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

#endif //MI_PDP2019_STACKITEM_H
