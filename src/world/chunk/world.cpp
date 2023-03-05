/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/chunk/world.h>
#include <blt/profiling/profiler.h>
#include <blt/std/queue.h>
#include <queue>
#include <render/camera.h>
#include "stb/stb_perlin.h"
#include <blt/std/format.h>

void fp::world::generateFullMesh(mesh_storage* mesh, fp::chunk* chunk) {
    BLT_START_INTERVAL("Chunk Mesh", "Full Mesh");
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                auto*& storage = chunk->getBlockStorage();
                
                auto& block = fp::registry::get(storage->get({i, j, k}));
                
                auto texture_index = fp::registry::getTextureIndex(block.textureName);
                
                // The main chunk mesh can handle opaque and transparent textures. (Transparency will be discarded)
                if (block.visibility <= registry::TRANSPARENT_TEXTURE) {
                    if (!storage->checkBlockVisibility({i - 1, j, k}))
                        mesh->addFace(X_NEG, {i, j, k}, texture_index);
                    if (!storage->checkBlockVisibility({i + 1, j, k}))
                        mesh->addFace(X_POS, {i, j, k}, texture_index);
                    if (!storage->checkBlockVisibility({i, j - 1, k}))
                        mesh->addFace(Y_NEG, {i, j, k}, texture_index);
                    if (!storage->checkBlockVisibility({i, j + 1, k}))
                        mesh->addFace(Y_POS, {i, j, k}, texture_index);
                    if (!storage->checkBlockVisibility({i, j, k - 1}))
                        mesh->addFace(Z_NEG, {i, j, k}, texture_index);
                    if (!storage->checkBlockVisibility({i, j, k + 1}))
                        mesh->addFace(Z_POS, {i, j, k}, texture_index);
                }
            }
        }
    }
    
    chunk->markPartialComplete();
    BLT_END_INTERVAL("Chunk Mesh", "Full Mesh");
}

inline void checkEdgeFaces(
        fp::mesh_storage* mesh, fp::chunk* chunk, fp::chunk* neighbour, fp::face face,
        const fp::block_pos& pos, const fp::block_pos& neighbour_pos
) {
    auto*& storage = chunk->getBlockStorage();
    auto& block = fp::registry::get(storage->get(pos));
    auto texture_index = fp::registry::getTextureIndex(block.textureName);
    
    if (block.visibility <= fp::registry::TRANSPARENT_TEXTURE) {
        if (fp::registry::get(storage->get(neighbour_pos)).visibility)
            mesh->addFace(face, pos, texture_index);
    }
}

void fp::world::generateEdgeMesh(mesh_storage* mesh, fp::chunk* chunk) {
    BLT_START_INTERVAL("Chunk Mesh", "Edge Mesh");
    // don't try to regen the chunk mesh unless there is a chance all neighbours are not null
    if (chunk->getStatus() != chunk_update_status::NEIGHBOUR_CREATE)
        return;
    
    chunk_neighbours neighbours{};
    getNeighbours(chunk->getPos(), neighbours);
    
    
    // if none of the neighbours exist we cannot continue!
    for (auto* neighbour : neighbours.neighbours) {
        if (!neighbour)
            return;
    }
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            checkEdgeFaces(
                    mesh, chunk, neighbours[X_NEG], X_NEG, {0, i, j}, {CHUNK_SIZE - 1, i, j}
            );
            checkEdgeFaces(
                    mesh, chunk, neighbours[X_POS], X_POS, {CHUNK_SIZE - 1, i, j}, {0, i, j}
            );
            
            checkEdgeFaces(
                    mesh, chunk, neighbours[Y_NEG], Y_NEG, {i, 0, j}, {i, CHUNK_SIZE - 1, j}
            );
            checkEdgeFaces(
                    mesh, chunk, neighbours[Y_POS], Y_POS, {i, CHUNK_SIZE - 1, j}, {i, 0, j}
            );
            
            checkEdgeFaces(
                    mesh, chunk, neighbours[Z_NEG], Z_NEG, {i, j, 0}, {i, j, CHUNK_SIZE - 1}
            );
            checkEdgeFaces(
                    mesh, chunk, neighbours[Z_POS], Z_POS, {i, j, CHUNK_SIZE - 1}, {i, j, 0}
            );
        }
    }
    
    chunk->getStatus() = NONE;
    chunk->markComplete();
    BLT_END_INTERVAL("Chunk Mesh", "Edge Mesh");
}

void fp::world::generateChunkMesh(fp::chunk* chunk) {
    if (chunk->getMeshStorage() == nullptr)
        chunk->getMeshStorage() = new mesh_storage();
    
    if (chunk->getDirtiness() == FULL_MESH) { // full chunk mesh
        generateFullMesh(chunk->getMeshStorage(), chunk);
    }
    if (chunk->getDirtiness() == PARTIAL_MESH) { // partial chunk mesh (had null neighbours)
        generateEdgeMesh(chunk->getMeshStorage(), chunk);
    }
}

std::queue<fp::chunk_pos> chunks_to_generate{};

void fp::world::update() {
    auto target_delta = 1000000000 / std::stoi(fp::settings::get("FPS"));
    
    while (fp::window::getCurrentDelta() < target_delta) {
        if (chunks_to_generate.empty())
            break;
        const auto& front = chunks_to_generate.front();
        
        insertChunk(generateChunk(front));
        
        chunks_to_generate.pop();
    }
    
}

void fp::world::render(fp::shader& shader) {
    shader.use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, fp::registry::getTextureID());
    
    auto view_distance = std::stoi(fp::settings::get("VIEW_DISTANCE")) / 2;
    
    for (int i = -view_distance; i <= view_distance; i++) {
        for (int j = -view_distance; j <= view_distance; j++) {
            for (int k = -view_distance; k <= view_distance; k++) {
                // get the chunks around the player's camera
                const auto& pos = fp::camera::getPosition();
                int x = (int) pos.x();
                int y = (int) pos.y();
                int z = (int) pos.z();
                auto camera_chunk_pos = fp::_static::world_to_chunk({x, y, z});
                auto adjusted_chunk_pos = chunk_pos{camera_chunk_pos.x + i, // chunk x
                                                    camera_chunk_pos.y + j, // chunk y
                                                    camera_chunk_pos.z + k}; // chunk z
                // generate chunk if it doesn't exist
                auto* chunk = this->getChunk(adjusted_chunk_pos);
                if (!chunk) {
                    chunks_to_generate.push(adjusted_chunk_pos);
                    continue;
                }
    
                // check for mesh updates
                if (chunk->getDirtiness() > REFRESH) {
                    BLT_START_INTERVAL("Chunk Mesh", "Mesh");
                    generateChunkMesh(chunk);
                    BLT_END_INTERVAL("Chunk Mesh", "Mesh");
                } else if (chunk->getDirtiness() == REFRESH) {
                    // 11436 vert, 137,232 bytes
                    // 1908 vert, 11436 indices, 22896 + 45744 = 68,640 bytes
                    chunk->updateChunkMesh();
                }
    
                chunk->render(shader);
            }
        }
    }
}

fp::chunk* fp::world::generateChunk(const fp::chunk_pos& pos) {
    if (this->getChunk(pos))
        return nullptr;
    BLT_START_INTERVAL("Chunk Generate", "Instantiate");
    auto* c = new chunk(pos);
    block_storage*& storage = c->getBlockStorage();
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                auto block_x = pos.x + i;
                auto block_y = pos.y + j;
                auto block_z = pos.z + k;
                storage->set(
                        {i, j, k},
                        (int) (stb_perlin_fbm_noise3(block_x / 8.0, block_y / 8.0,
                                                     block_z / 8.0, 2.0, 0.5, 6
                        ) > 0.5 ? fp::registry::STONE : fp::registry::AIR));
            }
        }
    }
    
    c->markDirty();
    
    BLT_END_INTERVAL("Chunk Generate", "Instantiate");
    
    return c;
}

void fp::chunk::render(fp::shader& shader) {
    if (render_size > 0) {
        blt::mat4x4 translation{};
        translation.translate((float) pos.x * CHUNK_SIZE,
                              (float) pos.y * CHUNK_SIZE,
                              (float) pos.z * CHUNK_SIZE
        );
        shader.setMatrix("translation", translation);
        // bind the chunk's VAO
        chunk_vao->bind();
        // despite binding the element buffer at creation time, this is required.
        chunk_vao->getVBO(-1)->bind();
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glDrawElements(GL_TRIANGLES, (int) render_size, GL_UNSIGNED_INT, nullptr);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }
}

void fp::chunk::updateChunkMesh() {
    auto& vertices = mesh->getVertices();
    auto& indices = mesh->getIndices();
    
    BLT_DEBUG(
            "Chunk [%d, %d, %d] mesh updated with %d vertices and %d indices taking (%s, %s) bytes!",
            pos.x, pos.y, pos.z,
            vertices.size(), indices.size(), blt::string::fromBytes(vertices.size() * sizeof(vertex)).c_str(),
            blt::string::fromBytes(indices.size() * sizeof(unsigned int)).c_str());
    
    // upload the new vertices to the GPU
    chunk_vao->getVBO(0)->update(vertices);
    chunk_vao->getVBO(-1)->update(indices);
    render_size = indices.size();
    
    // delete the local chunk mesh memory, since we no longer need to store it.
    delete (mesh);
    mesh = nullptr;
    markDone();
}
