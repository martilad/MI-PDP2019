//
// Created by martilad on 3/14/19.
//
#include "../headers/helpers.h"

static int eval_pol(int number) {
    if (number == 1) return -6;
    if (number == 2) return -12;
    if (number == 3) return -18;
    if (number == 6) return -3;
    if (number == 7) return -9;
    if (number == 11) return 0;
    int mod = number % 5;
    if (mod == 0) return (number / 5) * 3;
    else if (mod == 1) return ((number - 16) / 5) * 3 + 4 * 2;
    else if (mod == 2) return ((number - 12) / 5) * 3 + 3 * 2;
    else if (mod == 3) return ((number - 8) / 5) * 3 + 2 * 2;
    else return ((number - 4) / 5) * 3 + 2;
}

