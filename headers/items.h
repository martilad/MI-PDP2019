//
// Created by martilad on 3/14/19.
//

#ifndef MI_PDP2019_ITEMS_H
#define MI_PDP2019_ITEMS_H

#include "cord.h"
#include <vector>

// items defined by relative coordinates
const std::vector <cord> L31 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0)};
const std::vector <cord> L32 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2)};
const std::vector <cord> L33 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(1, 2)};
const std::vector <cord> L34 = {cord(0, 0), cord(-1, 1), cord(-2, 1), cord(0, 1)};
const std::vector <cord> L35 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2)};
const std::vector <cord> L36 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(2, 1)};
const std::vector <cord> L37 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1)};
const std::vector <cord> L38 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(-1, 2)};

const std::vector <cord> L41 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(2, 0), cord(3, 0)};
const std::vector <cord> L42 = {cord(0, 0), cord(1, 0), cord(1, 1), cord(1, 2), cord(1, 3)};
const std::vector <cord> L43 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(0, 3), cord(1, 3)};
const std::vector <cord> L44 = {cord(0, 0), cord(0, 1), cord(-1, 1), cord(-2, 1), cord(-3, 1)};
const std::vector <cord> L45 = {cord(0, 0), cord(1, 0), cord(0, 1), cord(0, 2), cord(0, 3)};
const std::vector <cord> L46 = {cord(0, 0), cord(1, 0), cord(2, 0), cord(3, 0), cord(3, 1)};
const std::vector <cord> L47 = {cord(0, 0), cord(0, 1), cord(1, 1), cord(2, 1), cord(3, 1)};
const std::vector <cord> L48 = {cord(0, 0), cord(0, 1), cord(0, 2), cord(0, 3), cord(-1, 3)};

const std::vector <std::vector<cord>> items = {{}, L41, L42, L43, L44, L45, L46, L47, L48, L31, L32, L33, L34, L35, L36, L37,
                                     L38};

#endif //MI_PDP2019_ITEMS_H
