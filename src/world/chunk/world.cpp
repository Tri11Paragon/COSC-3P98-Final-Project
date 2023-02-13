/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/world.h>

void fp::world::generateChunkMesh(fp::chunk* chunk) {
    auto* meshStorage = new mesh_storage;
    
    if (chunk->dirtiness == FULL_MESH) { // full chunk mesh
        for (int i = 0; i < CHUNK_SIZE; i++) {
            for (int j = 0; j < CHUNK_SIZE; j++) {
                for (int k = 0; k < CHUNK_SIZE; k++) {
                    auto block = chunk->storage->get({i, j, k});
                    if (block != 0) {
                        meshStorage->addFace(X_NEG, {i, j, k});
                        meshStorage->addFace(X_POS, {i, j, k});
                        meshStorage->addFace(Y_NEG, {i, j, k});
                        meshStorage->addFace(Y_POS, {i, j, k});
                        meshStorage->addFace(Z_NEG, {i, j, k});
                        meshStorage->addFace(Z_POS, {i, j, k});
                    }
                }
            }
        }
    } else if (chunk->dirtiness == PARTIAL_MESH){ // partial chunk mesh (had null edges)
    
    }
    
    chunk->mesh = meshStorage;
    chunk->dirtiness = REFRESH;
}

void fp::world::update() {

}

void fp::world::render(fp::shader& shader) {
    shader.use();
    
    for (const auto& chunk_pair : chunk_storage) {
        auto chunk = chunk_pair.second;
    
        if (chunk->dirtiness > REFRESH){
            generateChunkMesh(chunk);
        }
        
        if (chunk->dirtiness == REFRESH){
            auto& vertices = chunk->mesh->getVertices();

            BLT_INFO("Chunk [%d, %d, %d] mesh updated with %d vertices and %d indices taking (%d, %d) bytes!",
                     chunk->pos.x, chunk->pos.y, chunk->pos.z,
                     vertices.size(), 0, vertices.size() * sizeof(float), 0 * sizeof(unsigned int));
    
            // upload the new vertices to the GPU
            chunk->chunk_vao->getVBO(0)->update(vertices);
            chunk->render_size = vertices.size();
    
            // delete the memory from the CPU.
            delete(chunk->mesh);
            chunk->mesh = nullptr;
            chunk->dirtiness = OKAY;
        }
        
        if (chunk->render_size > 0){
            blt::mat4x4 translation {};
            translation.translate((float)chunk->pos.x * CHUNK_SIZE, (float)chunk->pos.y * CHUNK_SIZE, (float)chunk->pos.z * CHUNK_SIZE);
            shader.setMatrix("translation", translation);
            chunk->chunk_vao->bind();
            glEnableVertexAttribArray(0);
            //glDrawElements(GL_TRIANGLES, render_size, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, (int)chunk->render_size);
            glDisableVertexAttribArray(0);
        }
    }
}
