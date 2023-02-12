/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/storage.h>
#include <unordered_map>

constexpr float scale = 0.5f;

const float x_positive_vertices[VTX_ARR_SIZE] = {
        scale, scale, scale,  // +x top right
        scale, scale, -scale,  // +x bottom right
        scale, -scale, -scale,  // +x bottom left
        scale, -scale, scale   // +x top left
};
const float x_negative_vertices[VTX_ARR_SIZE] = {
        -scale, scale, scale,  // -x top right
        -scale, scale, -scale,  // -x bottom right
        -scale, -scale, -scale,  // -x bottom left
        -scale, -scale, scale   // -x top left
};
const float y_positive_vertices[VTX_ARR_SIZE] = {
        scale, scale, scale,  // +y top right
        -scale, scale, scale,  // +y bottom right
        -scale, scale, -scale,  // +y bottom left
        scale, scale, -scale,   // +y top left
};
const float y_negative_vertices[VTX_ARR_SIZE] = {
        scale, -scale, scale,  // -y top right
        -scale, -scale, scale,  // -y bottom right
        -scale, -scale, -scale,  // -y bottom left
        scale, -scale, -scale,    // -y top left
};
const float z_positive_vertices[VTX_ARR_SIZE] = {
        scale, scale, scale,  // +z top right
        scale, -scale, scale,  // +z bottom right
        -scale, -scale, scale,  // +z bottom left
        -scale, scale, scale   // +z top left
};
const float z_negative_vertices[VTX_ARR_SIZE] = {
        scale, scale, -scale,  // -z top right
        scale, -scale, -scale,  // -z bottom right
        -scale, -scale, -scale,  // -z bottom left
        -scale, scale, -scale   // -z top left
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

// indices are the same on all axis but are flipped between negative / positive as a result of back-face culling.
const std::vector<unsigned int> negative_indices = {
        0, 1, 3,
        1, 2, 3
};
const std::vector<unsigned int> positive_indices = {
        3, 1, 0,
        3, 2, 1
};

void fp::mesh_storage::addFace(fp::face face, float x, float y, float z) {
    BLT_DEBUG("Creating face %d ", face);
    add_and_translate(face_decode[face], x, y, z);
    
    // 3 floats per vertex, 4 vertices per face
    
    // 576 + 288 = 864 bytes for vertex + index
    // 12 faces * 6 vertex * 3 floats * 4 bytes = 864 bytes.
    
    // no savings but extra complexity
//    auto indice_offset = (vertices.size()/3 - 4);
//    auto& face_indices = face % 2 == 0 ? positive_indices : negative_indices;
//
//    for (unsigned int indice : face_indices)
//        indices.push_back(indice_offset + indice);
}
