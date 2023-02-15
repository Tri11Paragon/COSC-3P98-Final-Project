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
            return {world_to_internal(coord.x), world_to_internal(coord.y), world_to_internal(coord.z)};
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
        public:
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
                // using indices uses:
                // 12 faces * 4 vertex * 3 float * 4 bytes + 12 faces * 6 indices * 4 bytes = 864 bytes for vertex + index
                
                // using only vertices:
                // 12 faces * 6 vertex * 3 floats * 4 bytes = 864 bytes.
                
                // since they both use the same amount of memory we will only store the vertices and draw with drawArrays, since it is less complex.
                // set up the VBOs which will be later updated when the mesh is generated.
                chunk_vao->bindVBO(new VBO(ARRAY_BUFFER, nullptr, 0), 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
                chunk_vao->bindElementVBO(new VBO(ELEMENT_BUFFER, nullptr, 0));
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
            
            inline void getNeighbours(const chunk_pos& pos, chunk_neighbours& neighbours) {
                neighbours[X_POS] = getChunk(chunk_pos{pos.x + 1, pos.y, pos.z});
                neighbours[X_NEG] = getChunk(chunk_pos{pos.x - 1, pos.y, pos.z});
                neighbours[Y_POS] = getChunk(chunk_pos{pos.x, pos.y + 1, pos.z});
                neighbours[Y_NEG] = getChunk(chunk_pos{pos.x, pos.y - 1, pos.z});
                neighbours[Z_POS] = getChunk(chunk_pos{pos.x, pos.y, pos.z + 1});
                neighbours[Z_NEG] = getChunk(chunk_pos{pos.x, pos.y, pos.z - 1});
            }
            
            inline void insertChunk(chunk* chunk) {
                chunk_storage.insert({chunk->pos, chunk});
                
                chunk_neighbours chunkNeighbours{};
                getNeighbours(chunk->pos, chunkNeighbours);
                
                for (auto* p : chunkNeighbours.neighbours){
                    if (p)
                        p->status = NEIGHBOUR_CREATE;
                }
            }
            
            inline chunk* getChunk(const chunk_pos& pos){
                const auto map_pos = chunk_storage.find(pos);
                if (map_pos == chunk_storage.end())
                    return nullptr;
                return map_pos->second;
            }
            
            inline chunk* getChunk(const block_pos& pos) {
                return chunk_storage.at(_static::world_to_chunk(pos));
            }
        
        public:
            world() {
                insertChunk(new chunk({0, 0, 0}));
                insertChunk(new chunk({0, 1, 0}));
                insertChunk(new chunk({0, -1, 0}));
                insertChunk(new chunk({1, 0, 0}));
                insertChunk(new chunk({0, 0, 1}));
                insertChunk(new chunk({-1, 0, 0}));
                insertChunk(new chunk({0, 0, -1}));
                insertChunk(new chunk({-1, 0, -1}));
            }
            
            void update();
            
            void render(fp::shader& shader);
            
            inline bool setBlock(const block_pos& pos, block_type blockID) {
                auto c = getChunk(pos);
                if (!c)
                    return false;
                // mark the chunk for a mesh update
                c->dirtiness = FULL_MESH;
                c->storage->set(_static::world_to_internal(pos), blockID);
                return true;
            }
            
            inline block_type getBlock(const block_pos& pos) {
                auto c = getChunk(pos);
                if (!c)
                    return fp::registry::AIR;
                return c->storage->get(_static::world_to_internal(pos));
            }
            
            ~world() {
                for (auto& chunk : chunk_storage)
                    delete (chunk.second);
            }
    };
    
}

#endif //FINALPROJECT_WORLD_H
