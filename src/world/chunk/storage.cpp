/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/storage.h>

//volatile size_t total = 0;
//
//void* operator new(size_t size){
//    std::cout << "Creating new of size " << size << " bytes, total (" << total << ")\n" ;
//    total += size;
//    return malloc(size);
//}
//
//void operator delete(void* mem, size_t size){
//    std::cout << "Deleting " << size << " bytes, total (" << total << ")\n";
//    total -= size;
//    free(mem);
//}

constexpr float scale = 0.5f;

const fp::unpacked_vertex x_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale, scale,  scale, 1, 1, 0},  // +x top right
        {scale, scale,  -scale, 1, 0, 0},  // +x bottom right
        {scale, -scale, -scale, 0, 0, 0},  // +x bottom left
        {scale, -scale, scale, 0, 1, 0}   // +x top left
};
const fp::unpacked_vertex x_negative_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {-scale, scale,  scale, 1, 1, 0},  // -x top right
        {-scale, scale,  -scale, 1, 0, 0},  // -x bottom right
        {-scale, -scale, -scale, 0, 0, 0},  // -x bottom left
        {-scale, -scale, scale, 0, 1, 0}   // -x top left
};
const fp::unpacked_vertex y_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  scale, scale, 1, 1, 0},  // +y top right
        {-scale, scale, scale, 1, 0, 0},  // +y bottom right
        {-scale, scale, -scale, 0, 0, 0},  // +y bottom left
        {scale,  scale, -scale, 0, 1, 0},   // +y top left
};
const fp::unpacked_vertex y_negative_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  -scale, scale, 1, 1, 0},  // -y top right
        {-scale, -scale, scale, 1, 0, 0},  // -y bottom right
        {-scale, -scale, -scale, 0, 0, 0},  // -y bottom left
        {scale,  -scale, -scale, 0, 1, 0},    // -y top left
};
const fp::unpacked_vertex z_positive_vertices[VTX_ARR_SIZE] = {
        // x,   y,      z,     u, v, index
        {scale,  scale,  scale, 1, 1, 0},  // +z top right
        {scale,  -scale, scale, 1, 0, 0},  // +z bottom right
        {-scale, -scale, scale, 0, 0, 0},  // +z bottom left
        {-scale, scale,  scale, 0, 1, 0},   // +z top left
};
const fp::unpacked_vertex z_negative_vertices[VTX_ARR_SIZE] = {
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
const fp::unpacked_vertex* face_decode[] = {
        x_positive_vertices,
        x_negative_vertices,
        y_positive_vertices,
        y_negative_vertices,
        z_positive_vertices,
        z_negative_vertices
};

void fp::mesh_storage::addFace(fp::face face, const block_pos& pos, unsigned char texture_index) {
    constexpr int texture_index_loc = 32 - 8;
    constexpr int uv_index_loc = texture_index_loc - 2;
    constexpr int x_coord_loc = uv_index_loc - 6;
    constexpr int y_coord_loc = x_coord_loc - 6;
    constexpr int z_coord_loc = y_coord_loc - 6;
    
    const auto* face_vertices = face_decode[face];
    // negatives are odd numbered, positives are even.
    const auto& face_indices = face % 2 == 0 ? positive_indices : negative_indices;
    
    vertex translated_face_vertices[VTX_ARR_SIZE];
    
    // generate translated vertices
    for (int i = 0; i < VTX_ARR_SIZE; i++) {
        // convert from 2d uv coord to 1d index, based on the uv itself to itself!!
        int uv_index = (int)(face_vertices[i].u + face_vertices[i].v * 2);
        
        int data = 0;
        data = data | (texture_index << texture_index_loc);
        
        data = data | (uv_index << uv_index_loc);
        data = data | ((pos.x + (face_vertices[i].x > 0 ? 1 : 0)) << x_coord_loc);
        data = data | ((pos.y + (face_vertices[i].y > 0 ? 1 : 0)) << y_coord_loc);
        data = data | ((pos.z + (face_vertices[i].z > 0 ? 1 : 0)) << z_coord_loc);
        
        // the famous evil bit hack to convert types while maintaining the bits
        translated_face_vertices[i].data = *reinterpret_cast<float*>(&data);
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
            created_vertices_index[index_vertex] = current_index_pos;
            indices.push_back(current_index_pos);
        } else {
            // does exist in the face_vertices we can use that knowledge to reduce the total # of vertices
            indices.push_back(find_existing_vertex->second);
        }
    }
}
