/*
 * Created by Brett on 10/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_GRAPHICS_H
#define FINALPROJECT_GRAPHICS_H

#include <render/ui/text.h>
#include <render/renderer.h>

/**
 * fp::graphics will likely become the basis for blt::graphics in the future
 *
 * So let's go over some design decisions:
 *  - Immediate mode, similar in style of ImGUI
 *  - Functions can be called from anywhere,
 *  - One function to render all graphics currently requested
 *  - 2D support (UI: Rects, Lines, Points, Triangles, etc)
 *  - 3D support (3D text? Plane rendering, Lines, Points, etc)
 */
namespace fp::graphics {
    
    void drawPlane(const blt::vec4& plane, const blt::vec3& color);
    
    void init(renderer& renderer);
    
    void render();
    
    void cleanup();
    
}

#endif //FINALPROJECT_GRAPHICS_H
