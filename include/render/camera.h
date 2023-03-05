/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_CAMERA_H
#define FINALPROJECT_CAMERA_H

#include <render/window.h>

namespace fp::camera {
    void update();
    const blt::vec3& getPosition();
    const blt::vec3& getRotation();
}

#endif //FINALPROJECT_CAMERA_H
