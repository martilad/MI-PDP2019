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

    Item(){

    }

    Item(solution sol, cord co, int cnt) {
        this->sol = sol;
        this->co = co;
        this->cnt = cnt;
    }

    Item(solution sol, cord co, int cnt, int best) {
        this->sol = sol;
        this->co = co;
        this->cnt = cnt;
        this->bestScore = best;
    }

    Item(int * from){
        this->sol.m = from[0];
        this->sol.n = from[1];
        this->sol.k = from[2];
        this->sol.price = from[3];
        this->sol.actPrice = from[4];
        this->sol.nL3 = from[5];
        this->sol.nL4 = from[6];
        this->sol.nEmptyBefore = from[7];
        this->sol.nEmptyAfter = from[8];
        this->cnt = from[9];
        this->co.x = from[10];
        this->co.y = from[11];
        this->bestScore = from[12];
        this->sol.ground = std::vector < std::vector < int >> ();
        // init the ground
        int c = 14;
        for (int i = 0; i < this->sol.m; i++) {
            std::vector<int> tmp = std::vector<int>();
            for (int j = 0; j < this->sol.n; j++) {
                tmp.push_back(from[c++]);
            }
            this->sol.ground.push_back(tmp);
        }
    }

    solution sol;
    cord co;
    int cnt;
    int bestScore;

    void toMessage(int * message, int rank) {
        message[0] = this->sol.m;
        message[1] = this->sol.n;
        message[2] = this->sol.k;
        message[3] = this->sol.price;
        message[4] = this->sol.actPrice;
        message[5] = this->sol.nL3;
        message[6] = this->sol.nL4;
        message[7] = this->sol.nEmptyBefore;
        message[8] = this->sol.nEmptyAfter;
        message[9] = this->cnt;
        message[10] = this->co.x;
        message[11] = this->co.y;
        message[12] = this->bestScore;
        message[13] = rank;
        int c = 14;
        for (int i = 0; i < this->sol.m; i++) {
            for (int j = 0; j < this->sol.n; j++) {
                message[c++] = this->sol.ground[i][j];
            }
        }
    }
};

#endif //MI_PDP2019_STACKITEM_H
