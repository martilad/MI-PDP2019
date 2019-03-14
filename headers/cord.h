//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_CORD_H
#define MI_PDP2019_CORD_H

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

#endif //MI_PDP2019_CORD_H
