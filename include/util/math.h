/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_MATH_H
#define FINALPROJECT_MATH_H

#include <cmath>
#include <stb/stb_perlin.h>
#include <blt/math/math.h>
#include <blt/std/logging.h>

static inline float degreesToRadian(float deg) {
    return deg * (float)M_PI / 180.0f;
}

#endif //FINALPROJECT_MATH_H
