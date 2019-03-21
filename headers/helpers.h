//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_HELPERS_H
#define MI_PDP2019_HELPERS_H

#include <stdio.h>
#include <string.h>
#include <iostream>

namespace helpers {
    /**
     * Count the maximal price for the "number" of unsolved squares.
     *
     * @param number number of unsolved squares
     * @return returns the maximal price for the "number" of unsolved squares
     */
    int eval_pol(int number);


    void load_arguments(char ** file, int *run, int *nT, int *nN, int *nNP, int argc, char* argv[]);


}
#endif //MI_PDP2019_HELPERS_H
