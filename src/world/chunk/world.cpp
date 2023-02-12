/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/world.h>


//void fp::chunk::render(fp::shader& chunk_shader) {
//    // process mesh updates
//    if (isDirty){
//        auto* new_mesh = generateMesh();
//
//        auto& vertices = new_mesh->getVertices();
//        //auto& indices = new_mesh->getIndices();
//
//        BLT_INFO("Chunk mesh updated with %d vertices and %d indices taking (%d, %d) bytes!", vertices.size(), 0, vertices.size() * sizeof(float), 0 * sizeof(unsigned int));
//
//        chunk_vao->getVBO(0)->update(vertices);
//        //chunk_vao->getVBO(-1)->update(indices);
//
//        render_size = vertices.size();
//
//        delete(new_mesh);
//        isDirty = false;
//    }
//
//    if (render_size > 0){
//        chunk_vao->bind();
//        glEnableVertexAttribArray(0);
//        //glDrawElements(GL_TRIANGLES, render_size, GL_UNSIGNED_INT, nullptr);
//        glDrawArrays(GL_TRIANGLES, 0, render_size);
//        glDisableVertexAttribArray(0);
//    }
//
//}

fp::mesh_storage* fp::world::generateChunkMesh(fp::chunk* chunk) {
    auto* meshStorage = new mesh_storage;
    
    if (chunk->dirtiness > 1) { // full chunk mesh
        for (int i = 0; i < CHUNK_SIZE; i++) {
            for (int j = 0; j < CHUNK_SIZE; j++) {
                for (int k = 0; k < CHUNK_SIZE; k++) {
                    auto block = chunk->storage->get(i, j, k);
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
    } else if (chunk->dirtiness > 0){ // partial chunk mesh (had null edges)
    
    }
    
    return meshStorage;
}

void fp::world::update() {

}

void fp::world::render(fp::shader& shader) {
    shader.use();
    
    for (const auto& chunk_pair : chunk_storage) {
        auto chunk = chunk_pair.second;
        
        if (chunk->dirtiness > 0){
            auto mesh = generateChunkMesh(chunk);
            auto& vertices = mesh->getVertices();

            BLT_INFO("Chunk mesh updated with %d vertices and %d indices taking (%d, %d) bytes!", vertices.size(), 0, vertices.size() * sizeof(float), 0 * sizeof(unsigned int));
    
            chunk->chunk_vao->getVBO(0)->update(vertices);
            //chunk_vao->getVBO(-1)->update(indices);
    
            chunk->render_size = vertices.size();
    
            delete(mesh);
            chunk->dirtiness = 0;
        }
        
        if (chunk->render_size > 0){
            chunk->chunk_vao->bind();
            glEnableVertexAttribArray(0);
            //glDrawElements(GL_TRIANGLES, render_size, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, (int)chunk->render_size);
            glDisableVertexAttribArray(0);
        }
    }
}