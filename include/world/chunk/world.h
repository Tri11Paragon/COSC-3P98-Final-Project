/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_WORLD_H
#define FINALPROJECT_WORLD_H

#include <world/chunk/storage.h>
#include <render/gl.h>
#include <unordered_map>
#include "blt/profiling/profiler.h"

namespace fp {
    
    namespace _static {
        /**
         * Converts from world coord to chunk-internal coords
         * @param coord world space coordinate
         * @return chunk internal coord
         */
        static inline int world_to_internal(int coord) {
            auto val = coord % CHUNK_SIZE;
            return val < 0 ? CHUNK_SIZE + val : val;
        }
        
        static inline block_pos world_to_internal(const block_pos& coord) {
            return {world_to_internal(coord.x), world_to_internal(coord.y),
                    world_to_internal(coord.z)};
        }
        
        /**
         * Converts from world coord to chunk pos coords
         *
         * consider: (int) (-31 / 32) which equals 0
         * but a negative chunk would be stored at -1, not 0 (since that is taken by the positive coord chunk)
         * an arithmetic right shift would produce the desired -1 (see Java, which performs a signed right bit shift)
         * however in C++ shifting on a signed type is undefined behaviour. So we must emulate an arithmetic right shift.
         *
         * @param coord x,y, or z coordinate to convert
         * @return a right arithmetic bit shift resulting in a signed division of the coordinate by CHUNK_SIZE
         */
        static inline int world_to_chunk(int coord) {
            auto ucoord = (unsigned int) coord;
            
            ucoord >>= CHUNK_SHIFT;
            
            if (coord < 0) {
                // the mask only has to be generated once since it is never modified at runtime beyond assignment
                static unsigned int mask = 0;
                if (mask == 0) {
                    for (int i = 0; i < CHUNK_SHIFT; i++)
                        mask |= (1 << ((sizeof(int) * 8 - 1) - i));
                }
                ucoord |= mask;
            }
            
            return (int) (ucoord);
        }
        
        static inline chunk_pos world_to_chunk(const block_pos& pos) {
            return {world_to_chunk(pos.x), world_to_chunk(pos.y), world_to_chunk(pos.z)};
        }
        
    }
    
    struct chunk {
        private:
            block_storage* storage;
            mesh_storage* mesh = nullptr;
            VAO* chunk_vao;
            chunk_pos pos;
            
            chunk_mesh_status dirtiness = OKAY;
            chunk_update_status status = NONE;
            unsigned long render_size = 0;
        public:
            explicit chunk(chunk_pos pos): pos(pos) {
                storage = new block_storage();
                chunk_vao = new VAO();
                auto vbo = new VBO(ARRAY_BUFFER, nullptr, 0);
                auto data_size = 3 * sizeof(float) + 3 * sizeof(float);
                chunk_vao->bindVBO(vbo, 0, 3, GL_FLOAT, (int) data_size, 0);
                chunk_vao->bindVBO(vbo, 1, 3, GL_FLOAT, (int) data_size, 3 * sizeof(float), true);
                chunk_vao->bindElementVBO(new VBO(ELEMENT_BUFFER, nullptr, 0));
            }
            
            void render(shader& shader);
            
            void updateChunkMesh();
            
            /**
             * Mark the chunk as completely dirty and in need of a full check refresh
             */
            inline void markDirty() {
                dirtiness = FULL_MESH;
            }
            
            /**
             * Partial mesh update has been completed, we are now waiting on the edge chunks to be
             * generated before continuing to generate the chunk edge mesh
             */
            inline void markPartialComplete() {
                dirtiness = PARTIAL_MESH;
            }
            
            /**
             * Full chunk mesh is now completely generated and waiting on uploading to the GPU
             */
            inline void markComplete() {
                dirtiness = REFRESH;
            }
            
            /**
             * Mesh uploading complete, chunk meshing is now done and inactive
             */
            inline void markDone() {
                dirtiness = OKAY;
            }
            
            [[nodiscard]] inline block_storage*& getBlockStorage() {
                return storage;
            }
            
            [[nodiscard]] inline mesh_storage*& getMeshStorage() {
                return mesh;
            }
            
            [[nodiscard]] inline VAO*& getVAO() {
                return chunk_vao;
            }
            
            [[nodiscard]] inline chunk_pos getPos() const {
                return pos;
            }
            
            [[nodiscard]] inline chunk_mesh_status getDirtiness() const {
                return dirtiness;
            }
            
            [[nodiscard]] inline chunk_update_status& getStatus() {
                return status;
            }
            
            inline void setStatus(const chunk_update_status& new_status) {
                status = new_status;
            }
            
            ~chunk() {
                delete storage;
                delete chunk_vao;
                delete mesh;
            }
    };
    
    struct chunk_neighbours {
        fp::chunk* neighbours[6];
        
        inline chunk*& operator[](int i) {
            return neighbours[i];
        }
    };
    
    class world {
        private:
            std::unordered_map<chunk_pos, chunk*, _static::chunk_pos_hash, _static::chunk_pos_equality> chunk_storage;
        protected:
            static void generateFullMesh(mesh_storage* mesh, chunk* chunk);
            
            void generateEdgeMesh(mesh_storage* mesh, chunk* chunk);
            
            void generateChunkMesh(chunk* chunk);
            
            chunk* generateChunk(const chunk_pos& pos);
            
            inline void getNeighbours(const chunk_pos& pos, chunk_neighbours& neighbours) {
                neighbours[X_POS] = getChunk(chunk_pos{pos.x + 1, pos.y, pos.z});
                neighbours[X_NEG] = getChunk(chunk_pos{pos.x - 1, pos.y, pos.z});
                neighbours[Y_POS] = getChunk(chunk_pos{pos.x, pos.y + 1, pos.z});
                neighbours[Y_NEG] = getChunk(chunk_pos{pos.x, pos.y - 1, pos.z});
                neighbours[Z_POS] = getChunk(chunk_pos{pos.x, pos.y, pos.z + 1});
                neighbours[Z_NEG] = getChunk(chunk_pos{pos.x, pos.y, pos.z - 1});
            }
            
            inline void insertChunk(chunk* chunk) {
                if (chunk == nullptr)
                    return;
                chunk_storage.insert({chunk->getPos(), chunk});
                
                chunk_neighbours chunkNeighbours{};
                getNeighbours(chunk->getPos(), chunkNeighbours);
                
                for (auto* p : chunkNeighbours.neighbours) {
                    if (p)
                        p->setStatus(NEIGHBOUR_CREATE);
                }
            }
            
            inline chunk* getChunk(const chunk_pos& pos) {
                const auto map_pos = chunk_storage.find(pos);
                if (map_pos == chunk_storage.end())
                    return nullptr;
                return map_pos->second;
            }
            
            inline chunk* getChunk(const block_pos& pos) {
                return chunk_storage[_static::world_to_chunk(pos)];
            }
        
        public:
            world() = default;
            
            void update();
            
            void render(fp::shader& shader);
            
            inline bool setBlock(const block_pos& pos, block_type blockID) {
                auto c = getChunk(pos);
                if (!c)
                    return false;
                // mark the chunk for a mesh update
                c->markDirty();
                c->getBlockStorage()->set(_static::world_to_internal(pos), blockID);
                return true;
            }
            
            inline block_type getBlock(const block_pos& pos) {
                auto c = getChunk(pos);
                if (!c)
                    return fp::registry::AIR;
                return c->getBlockStorage()->get(_static::world_to_internal(pos));
            }
            
            ~world() {
                BLT_PRINT_PROFILE("Chunk Mesh", blt::logging::TRACE, true);
                BLT_PRINT_PROFILE("Chunk Generate", blt::logging::TRACE, true);
                for (auto& chunk : chunk_storage)
                    delete (chunk.second);
            }
    };
    
}

#endif //FINALPROJECT_WORLD_H
