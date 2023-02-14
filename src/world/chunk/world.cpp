/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/world.h>

void fp::world::generateFullMesh(mesh_storage* mesh, fp::chunk* chunk) {
    // checks to outside the bounds of the chunk should not have faces added. this will be handled by the partial mesh!
    bool outside = false;
    
    for (int i = 1; i < CHUNK_SIZE - 1; i++) {
        for (int j = 1; j < CHUNK_SIZE - 1; j++) {
            for (int k = 1; k < CHUNK_SIZE - 1; k++) {
                auto block = chunk->storage->get({i, j, k});
                // opaque visibility is always 0. Non-zero values (true) are what we care about since opaque blocks are completely hidden
                if (!fp::registry::get(block).visibility) {
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i - 1, j, k})).visibility && !outside)
                        mesh->addFace(X_NEG, {i, j, k});
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i + 1, j, k})).visibility && !outside)
                        mesh->addFace(X_POS, {i, j, k});
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i, j - 1, k})).visibility && !outside)
                        mesh->addFace(Y_NEG, {i, j, k});
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i, j + 1, k})).visibility && !outside)
                        mesh->addFace(Y_POS, {i, j, k});
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i, j, k - 1})).visibility && !outside)
                        mesh->addFace(Z_NEG, {i, j, k});
                    if (fp::registry::get(chunk->storage->getBounded(outside, {i, j, k + 1})).visibility && !outside)
                        mesh->addFace(Z_POS, {i, j, k});
                }
            }
        }
    }
    
    chunk->dirtiness = PARTIAL_MESH;
}

inline void checkEdgeFaces(
        fp::mesh_storage* mesh, fp::chunk* chunk, fp::chunk* neighbour, fp::face face, const fp::block_pos& pos, const fp::block_pos& neighbour_pos
) {
    auto block = chunk->storage->get(pos);
    if (!fp::registry::get(block).visibility) {
        if (fp::registry::get(neighbour->storage->get(neighbour_pos)).visibility)
            mesh->addFace(face, pos);
    }
}

void fp::world::generateEdgeMesh(mesh_storage* mesh, fp::chunk* chunk) {
    BLT_TRACE("NOPE");
    // don't try to regen the chunk mesh unless there is a chance all neighbours are not null
    if (chunk->status != chunk_update_status::NEIGHBOUR_CREATE)
        return;
    
    chunk_neighbours neighbours{};
    getNeighbours(chunk->pos, neighbours);
    
    BLT_TRACE("GOODBYE");
    
    // if none of the neighbours exist we cannot continue!
    for (auto* neighbour : neighbours.neighbours) {
        if (!neighbour)
            return;
    }
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            checkEdgeFaces(mesh, chunk, neighbours[X_NEG], X_NEG, {0, i, j}, {CHUNK_SIZE - 1, i, j});
            checkEdgeFaces(mesh, chunk, neighbours[X_POS], X_POS, {CHUNK_SIZE-1, i, j}, {0, i, j});
    
            checkEdgeFaces(mesh, chunk, neighbours[Y_NEG], Y_NEG, {i, 0, j}, {i, CHUNK_SIZE - 1, j});
            checkEdgeFaces(mesh, chunk, neighbours[Y_POS], Y_POS, {i, CHUNK_SIZE-1, j}, {i, 0, j});
    
            checkEdgeFaces(mesh, chunk, neighbours[Z_NEG], Z_NEG, {i, j, 0}, {i, j, CHUNK_SIZE - 1});
            checkEdgeFaces(mesh, chunk, neighbours[Z_POS], Z_POS, {i, j, CHUNK_SIZE-1}, {i, j, 0});
        }
    }
    
    BLT_TRACE("HELLO");
    
    chunk->status = NONE;
    chunk->dirtiness = REFRESH;
}

void fp::world::generateChunkMesh(fp::chunk* chunk) {
    if (chunk->mesh == nullptr)
        chunk->mesh = new mesh_storage;
    
    if (chunk->dirtiness == FULL_MESH) { // full chunk mesh
        generateFullMesh(chunk->mesh, chunk);
    }
    if (chunk->dirtiness == PARTIAL_MESH) { // partial chunk mesh (had null neighbours)
        generateEdgeMesh(chunk->mesh, chunk);
    }
    
    chunk->dirtiness = REFRESH;
}

void fp::world::update() {

}

void fp::world::render(fp::shader& shader) {
    shader.use();
    
    for (const auto& chunk_pair : chunk_storage) {
        auto chunk = chunk_pair.second;
        
        if (chunk == nullptr)
            continue;
        
        if (chunk->dirtiness > REFRESH) {
            generateChunkMesh(chunk);
        }
        
        if (chunk->dirtiness == REFRESH) {
            auto& vertices = chunk->mesh->getVertices();
            
            BLT_INFO("Chunk [%d, %d, %d] mesh updated with %d vertices and %d indices taking (%d, %d) bytes!",
                     chunk->pos.x, chunk->pos.y, chunk->pos.z,
                     vertices.size(), 0, vertices.size() * sizeof(float), 0 * sizeof(unsigned int));
            
            // upload the new vertices to the GPU
            chunk->chunk_vao->getVBO(0)->update(vertices);
            chunk->render_size = vertices.size() / 3;
            
            // delete the memory from the CPU.
            delete (chunk->mesh);
            chunk->mesh = nullptr;
            chunk->dirtiness = OKAY;
        }
        
        if (chunk->render_size > 0) {
            blt::mat4x4 translation{};
            translation.translate((float) chunk->pos.x * CHUNK_SIZE, (float) chunk->pos.y * CHUNK_SIZE, (float) chunk->pos.z * CHUNK_SIZE);
            shader.setMatrix("translation", translation);
            chunk->chunk_vao->bind();
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLES, 0, (int) chunk->render_size);
            glDisableVertexAttribArray(0);
        }
    }
}
