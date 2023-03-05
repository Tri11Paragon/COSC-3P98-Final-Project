/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/storage.h>
#include <unordered_map>

constexpr float scale = 0.5f;

const fp::vertex x_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale, scale,  scale, 1, 1, 0},  // +x top right
        {scale, scale,  -scale, 1, 0, 0},  // +x bottom right
        {scale, -scale, -scale, 0, 0, 0},  // +x bottom left
        {scale, -scale, scale, 0, 1, 0}   // +x top left
};
const fp::vertex x_negative_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {-scale, scale,  scale, 1, 1, 0},  // -x top right
        {-scale, scale,  -scale, 1, 0, 0},  // -x bottom right
        {-scale, -scale, -scale, 0, 0, 0},  // -x bottom left
        {-scale, -scale, scale, 0, 1, 0}   // -x top left
};
const fp::vertex y_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  scale, scale, 1, 1, 0},  // +y top right
        {-scale, scale, scale, 1, 0, 0},  // +y bottom right
        {-scale, scale, -scale, 0, 0, 0},  // +y bottom left
        {scale,  scale, -scale, 0, 1, 0},   // +y top left
};
const fp::vertex y_negative_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  -scale, scale, 1, 1, 0},  // -y top right
        {-scale, -scale, scale, 1, 0, 0},  // -y bottom right
        {-scale, -scale, -scale, 0, 0, 0},  // -y bottom left
        {scale,  -scale, -scale, 0, 1, 0},    // -y top left
};
const fp::vertex z_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  scale,  scale, 1, 1, 0},  // +z top right
        {scale,  -scale, scale, 1, 0, 0},  // +z bottom right
        {-scale, -scale, scale, 0, 0, 0},  // +z bottom left
        {-scale, scale,  scale, 0, 1, 0},   // +z top left
};
const fp::vertex z_negative_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  scale,  -scale, 1, 1, 0},  // -z top right
        {scale,  -scale, -scale, 1, 0, 0},  // -z bottom right
        {-scale, -scale, -scale, 0, 0, 0},  // -z bottom left
        {-scale, scale,  -scale, 0, 1, 0},   // -z top left
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

// always ordered the same as the enum!
const fp::vertex* face_decode[] = {
        x_positive_vertices,
        x_negative_vertices,
        y_positive_vertices,
        y_negative_vertices,
        z_positive_vertices,
        z_negative_vertices
};

void fp::mesh_storage::addFace(fp::face face, const block_pos& pos, unsigned char texture_index) {
    const auto* face_vertices = face_decode[face];
    // negatives are odd numbered, positives are even.
    const auto& face_indices = face % 2 == 0 ? positive_indices : negative_indices;
    
    vertex translated_face_vertices[VTX_ARR_SIZE];
    
    // generate translated vertices
    for (int i = 0; i < VTX_ARR_SIZE; i++) {
        // first copy all the information over, since there is extra information we need to preserve like index and UV / normal
        translated_face_vertices[i] = face_vertices[i];
        translated_face_vertices[i].index = (float) texture_index;
        // then we can apply the translation, since the face_vertex value is already there we can add the translation raw
        translated_face_vertices[i].x += (float) pos.x;
        translated_face_vertices[i].y += (float) pos.y;
        translated_face_vertices[i].z += (float) pos.z;
    }
    
    for (unsigned int face_index : face_indices) {
        // vertex associated with the index
        auto index_vertex = translated_face_vertices[face_index];
        // search to see if the vertex exists already inside the vertices face_vertices
        auto find_existing_vertex = created_vertices_index.find(index_vertex);
        if (find_existing_vertex == created_vertices_index.end()) {
            // vertex doesn't already exist in the face_vertices.
            // the current size contains the position we are inserting the vertex into. This is our index in the vertex face_vertices
            auto current_index_pos = vertices.size();
            vertices.push_back(index_vertex);
            // Since we are inserting using the order of the face_indices this will ensure that the triangle vertices are ordered correctly (outward facing)
            created_vertices_index.insert({index_vertex, current_index_pos});
            indices.push_back(current_index_pos);
        } else {
            // does exist in the face_vertices we can use that knowledge to reduce the total # of vertices
            indices.push_back(find_existing_vertex->second);
        }
    }
}
