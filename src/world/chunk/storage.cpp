/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/storage.h>
#include <unordered_map>

constexpr float scale = 0.5f;

const float x_positive_vertices[VTX_ARR_SIZE] = {
        // +x first triangle
        scale, -scale,  scale,  // +x top left
        scale, scale, -scale,  // +x bottom right
        scale, scale,  scale,  // +x top right
        // +x second triangle
        scale, -scale,  scale,   // +x top left
        scale, -scale, -scale,  // +x bottom left
        scale, scale, -scale,  // +x bottom right
};
const float x_negative_vertices[VTX_ARR_SIZE] = {
        // -x first triangle
        -scale, scale,  scale,   // -x top right
        -scale, scale, -scale,   // -x bottom right
        -scale, -scale,  scale,  // -x top left
        // -x second triangle
        -scale, scale, -scale,   // -x bottom right
        -scale, -scale, -scale,  // -x bottom left
        -scale, -scale,  scale,   // -x top left
};
const float y_positive_vertices[VTX_ARR_SIZE] = {
        // first triangle
        scale, scale, -scale,   // top left
        -scale, scale, scale,   // bottom right
        scale, scale, scale,    // top right
        // second triangle
        scale, scale, -scale,   // top left
        -scale, scale, -scale,  // bottom left
        -scale, scale, scale,   // bottom right
};
const float y_negative_vertices[VTX_ARR_SIZE] = {
        // first triangle
        scale, -scale, scale,    // top right
        -scale, -scale, scale,   // bottom right
        scale, -scale, -scale,   // top left
        // second triangle
        -scale, -scale, scale,   // bottom right
        -scale, -scale, -scale,  // bottom left
        scale, -scale, -scale,   // top left
};
const float z_positive_vertices[VTX_ARR_SIZE] = {
        // first triangle
        -scale,  scale, scale,  // top left
        scale, -scale, scale,  // bottom right
        scale,  scale, scale,  // top right
        // second triangle
        -scale,  scale, scale,   // top left
        -scale, -scale, scale,  // bottom left
        scale, -scale, scale,  // bottom right
};
const float z_negative_vertices[VTX_ARR_SIZE] = {
        // first triangle
        scale,  scale, -scale,  // top right
        scale, -scale, -scale,  // bottom right
        -scale,  scale, -scale,  // top left
        // second triangle
        scale, -scale, -scale,  // bottom right
        -scale, -scale, -scale,  // bottom left
        -scale,  scale, -scale,   // top left
};

// always ordered the same as the enum!
const float* face_decode[] = {
        x_positive_vertices,
        x_negative_vertices,
        y_positive_vertices,
        y_negative_vertices,
        z_positive_vertices,
        z_negative_vertices
};

void fp::mesh_storage::addFace(fp::face face, const block_pos& pos) {
    add_and_translate(face_decode[face], pos);
}
