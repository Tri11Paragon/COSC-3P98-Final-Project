/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/chunk.h>

fp::mesh_storage* fp::chunk::generateMesh() {
    auto* meshStorage = new mesh_storage;
    
    for (int i = 0; i < CHUNK_SIZE; i++){
        for (int j = 0; j < CHUNK_SIZE; j++){
            for (int k = 0; k < CHUNK_SIZE; k++){
                auto block = storage->get(i,j,k);
                if (block != 0) {
                    meshStorage->addFace(X_NEG, i, j, k);
                    meshStorage->addFace(X_POS, i, j, k);
                    meshStorage->addFace(Y_NEG, i, j, k);
                    meshStorage->addFace(Y_POS, i, j, k);
                    meshStorage->addFace(Z_NEG, i, j, k);
                    meshStorage->addFace(Z_POS, i, j, k);
                }
            }
        }
    }
    
    return meshStorage;
}

void fp::chunk::update() {

}

void fp::chunk::render(fp::shader& chunk_shader) {
    // process mesh updates
    if (isDirty){
        auto* new_mesh = generateMesh();
        
        auto& vertices = new_mesh->getVertices();
        auto& indices = new_mesh->getIndices();
    
        BLT_INFO("Chunk mesh updated with %d vertices and %d indices taking (%d, %d) bytes!", vertices.size(), indices.size(), vertices.size() * sizeof(float), indices.size() * sizeof(unsigned int));
        
        chunk_vao->getVBO(0)->update(vertices);
        chunk_vao->getVBO(-1)->update(indices);
        
        render_size = indices.size();
        
        delete(new_mesh);
        isDirty = false;
    }
    
    if (render_size > 0){
        chunk_vao->bind();
        glEnableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, render_size, GL_UNSIGNED_INT, nullptr);
        glDisableVertexAttribArray(0);
    }
    
}
