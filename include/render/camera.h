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
    
    /**
     * Locks the current CPU sided view matrix, while allowing the camera to move
     * Mostly useful for testing frustum culling implementations as the view matrix will remain the same as the
     * last matrix before this function is called while allowing you to move around the scene
     */
    void freeze();
    
    void unfreeze();
    
    bool isFrozen();
    
    /**
     * @return the last non-frozen view matrix
     */
    const blt::mat4x4& getViewMatrix();
    /**
     * @return the last non-frozen projection * view matrix
     */
    const blt::mat4x4& getPVM();
}

#endif //FINALPROJECT_CAMERA_H
