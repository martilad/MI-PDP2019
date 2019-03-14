//
// Created by ladislav on 3/14/19.
//

#ifndef MI_PDP2019_RECURSION_H
#define MI_PDP2019_RECURSION_H

#include "solver.h"

class Recursion : public Solver {

protected:
    /**
     * Recursion with no copy, only pass the coordinates.
     *
     * @param co coordinates of do position
     * @param cnt id of item on map
     */
    void recursion(cord co, int cnt);

public:

    Recursion();
    virtual ~Recursion();
    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();

};


#endif //MI_PDP2019_RECURSION_H
