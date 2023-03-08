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
#include <fstream>
#include <ios>

inline void checkEdgeFace(
        fp::block_storage* local, fp::block_storage* neighbour,
        fp::mesh_storage* mesh, fp::face face,
        const fp::block_pos& pos, const fp::block_pos& neighbour_pos
) {
    auto& block = fp::registry::get(local->get(pos));
    
    if (block.visibility == fp::registry::OPAQUE) {
        if (fp::registry::get(neighbour->get(neighbour_pos)).visibility > fp::registry::OPAQUE) {
            mesh->addFace(face, pos, block.textureIndex);
        }
    }
}

void fp::world::generateChunkMesh(chunk* chunk) {
    // don't re-mesh unless requested
    if (chunk->getDirtiness() != DIRTY)
        return;
    // don't try to re-mesh the chunk unless there is a chance all neighbours are not null
    if (chunk->getStatus() != chunk_update_status::NEIGHBOUR_CREATE)
        return;
    
    chunk_neighbours neighbours{};
    getNeighbours(chunk->getPos(), neighbours);
    
    // if none of the neighbours exist we cannot continue!
    for (auto* neighbour : neighbours.neighbours) {
        if (!neighbour)
            return;
    }
    
    BLT_START_INTERVAL("Chunk Mesh", "Storage");
    
    auto* mesh = new mesh_storage();
    auto* local_storage = chunk->getBlockStorage();
    auto* x_neg_storage = neighbours[X_NEG]->getBlockStorage();
    auto* x_pos_storage = neighbours[X_POS]->getBlockStorage();
    auto* y_neg_storage = neighbours[Y_NEG]->getBlockStorage();
    auto* y_pos_storage = neighbours[Y_POS]->getBlockStorage();
    auto* z_neg_storage = neighbours[Z_NEG]->getBlockStorage();
    auto* z_pos_storage = neighbours[Z_POS]->getBlockStorage();
    
    BLT_END_INTERVAL("Chunk Mesh", "Storage");
    BLT_START_INTERVAL("Chunk Mesh", "Internal");
    
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                auto& block = fp::registry::get(local_storage->get({i, j, k}));
                
                auto texture_index = block.textureIndex;
                
                // The main chunk mesh can handle opaque textures.
                if (block.visibility == registry::OPAQUE) {
                    if (local_storage->checkBlockVisibility({i - 1, j, k}))
                        mesh->addFace(X_NEG, {i, j, k}, texture_index);
                    if (local_storage->checkBlockVisibility({i + 1, j, k}))
                        mesh->addFace(X_POS, {i, j, k}, texture_index);
                    if (local_storage->checkBlockVisibility({i, j - 1, k}))
                        mesh->addFace(Y_NEG, {i, j, k}, texture_index);
                    if (local_storage->checkBlockVisibility({i, j + 1, k}))
                        mesh->addFace(Y_POS, {i, j, k}, texture_index);
                    if (local_storage->checkBlockVisibility({i, j, k - 1}))
                        mesh->addFace(Z_NEG, {i, j, k}, texture_index);
                    if (local_storage->checkBlockVisibility({i, j, k + 1}))
                        mesh->addFace(Z_POS, {i, j, k}, texture_index);
                }
            }
        }
    }
    BLT_END_INTERVAL("Chunk Mesh", "Internal");
    BLT_START_INTERVAL("Chunk Mesh", "Partial");
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            checkEdgeFace(
                    local_storage, x_neg_storage, mesh, X_NEG, {0, i, j}, {CHUNK_SIZE - 1, i, j}
            );
            checkEdgeFace(
                    local_storage, x_pos_storage, mesh, X_POS, {CHUNK_SIZE - 1, i, j}, {0, i, j}
            );
            
            checkEdgeFace(
                    local_storage, y_neg_storage, mesh, Y_NEG, {i, 0, j}, {i, CHUNK_SIZE - 1, j}
            );
            checkEdgeFace(
                    local_storage, y_pos_storage, mesh, Y_POS, {i, CHUNK_SIZE - 1, j}, {i, 0, j}
            );
            
            checkEdgeFace(
                    local_storage, z_neg_storage, mesh, Z_NEG, {i, j, 0}, {i, j, CHUNK_SIZE - 1}
            );
            checkEdgeFace(
                    local_storage, z_pos_storage, mesh, Z_POS, {i, j, CHUNK_SIZE - 1}, {i, j, 0}
            );
        }
    }
    BLT_END_INTERVAL("Chunk Mesh", "Partial");
    
    chunk->getMeshStorage() = mesh;
    chunk->getStatus() = NONE;
    chunk->markRefresh();
    
    
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
                    generateChunkMesh(chunk);
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
    block_storage* storage = c->getBlockStorage();
    
    for (int i = 0; i < CHUNK_SIZE; i++) {
        auto block_x = float(pos.x * CHUNK_SIZE + i);
        for (int k = 0; k < CHUNK_SIZE; k++) {
            auto block_z = float(pos.z * CHUNK_SIZE + k);
            auto world_height = stb_perlin_ridge_noise3(
                    block_x / 128.0f,
                    8.1539123f,
                    block_z / 128.0f, 2.0f, 0.5f, 1.0, 12.0f
            ) * 128 + 64;
            
            for (int j = 0; j < CHUNK_SIZE; j++) {
                auto block_y = float(pos.y * CHUNK_SIZE + j);
                
                if (block_y < world_height)
                    storage->set({i, j, k}, fp::registry::STONE);
            }
        }
    }
    
    c->markDirty();
    
    BLT_END_INTERVAL("Chunk Generate", "Instantiate");
    
    return c;
}

fp::world::~world() {
    BLT_PRINT_PROFILE("Chunk Mesh", blt::logging::TRACE, true);
    std::ofstream profile{"decomposition_chunk.csv"};
    BLT_WRITE_PROFILE(profile, "Chunk Mesh");
    for (auto& chunk : chunk_storage)
        delete (chunk.second);
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
            vertices.size(), indices.size(),
            blt::string::fromBytes(vertices.size() * sizeof(vertex)).c_str(),
            blt::string::fromBytes(indices.size() * sizeof(unsigned int)).c_str());
    
    // upload the new vertices to the GPU
    chunk_vao->getVBO(0)->update(vertices);
    chunk_vao->getVBO(-1)->update(indices);
    render_size = indices.size();
    
    // delete the local chunk mesh memory, since we no longer need to store it.
    delete (mesh);
    mesh = nullptr;
    dirtiness = OKAY;
}
