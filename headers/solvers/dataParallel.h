//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_DATAPARALLEL_H
#define MI_PDP2019_DATAPARALLEL_H

#include <stack>
#include "solver.h"
#include "../item.h"


class DataParallel : public Solver {

protected:

    int nThreads;
    int generated;

    void generateInstBFS();

    void solveOneInst(Item it);

    void recursionSingleThreadDFS(solution * sol, cord co, int cnt);

public:

    DataParallel(int nThreads, int generated);
    virtual ~DataParallel();

    /**
     * Solve the map with items in possibility.
     */
    virtual void solve();





};

#endif //MI_PDP2019_DATAPARALLEL_H
