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
    
    struct ChunkPos {
        int x, y, z;
    };
    
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
        
        // std::unordered_map requires a type. As a result the functions are encapsulated.
        struct ChunkPosHash {
            inline size_t operator()(const ChunkPos& pos) const {
                size_t p1 = std::hash<int>()(pos.x);
                size_t p2 = std::hash<int>()(pos.y);
                size_t p3 = std::hash<int>()(pos.z);
                return (p1 ^ (p2 << 1)) ^ p3;
            }
        };
        
        struct ChunkPosEquality {
            inline bool operator()(const ChunkPos& p1, const ChunkPos& p2) const {
                return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
            }
        };
        
    }
    
    struct chunk {
        public:
            block_storage* storage;
            mesh_storage* mesh = nullptr;
            VAO* chunk_vao;
            ChunkPos pos;
            
            unsigned char dirtiness = 0;
            unsigned long render_size = 0;
        public:
            explicit chunk(ChunkPos pos): pos(pos) {
                storage = new block_storage();
                chunk_vao = new VAO();
                // using indices uses:
                // 12 faces * 4 vertex * 3 float * 4 bytes + 12 faces * 6 indices * 4 bytes = 864 bytes for vertex + index
                
                // using only vertices:
                // 12 faces * 6 vertex * 3 floats * 4 bytes = 864 bytes.
                
                // since they both use the same amount of memory we will only store the vertices and draw with drawArrays, since it is less complex.
                // set up the VBOs which will be later updated when the mesh is generated.
                chunk_vao->bindVBO(new VBO(ARRAY_BUFFER, nullptr, 0), 0, 3);
            }
            
            ~chunk() {
                delete storage;
                delete chunk_vao;
            }
    };
    
    class world {
        private:
            std::unordered_map<ChunkPos, chunk*, _static::ChunkPosHash, _static::ChunkPosEquality> chunk_storage;
        protected:
            mesh_storage* generateChunkMesh(chunk* chunk);
            
            chunk* getChunk(int x, int y, int z) {
                return chunk_storage.at(ChunkPos{_static::world_to_chunk(x), _static::world_to_chunk(y), _static::world_to_chunk(z)});
            }
        
        public:
            world() {
                chunk_storage.insert({{0, 0, 0}, new chunk({0, 0, 0})});
            }
            
            void update();
            
            void render(fp::shader& shader);
            
            inline void setBlock(int x, int y, int z, char blockID) {
                auto c = getChunk(x, y, z);
                // mark the chunk for a mesh update
                c->dirtiness = 2;
                c->storage->set(_static::world_to_internal(x), _static::world_to_internal(y), _static::world_to_internal(z), blockID);
            }
            
            inline void setBlock(float x, float y, float z, char blockID) { setBlock((int) x, (int) y, (int) z, blockID); }
            
            inline char getBlock(int x, int y, int z) {
                auto c = getChunk(x, y, z);
                return c->storage->get(x, y, z);
            }
            
            inline char getBlock(float x, float y, float z) {
                return getBlock((int) x, (int) y, (int) z);
            }
            
            ~world() {
                for (auto& chunk : chunk_storage)
                    delete (chunk.second);
            }
    };
    
}

#endif //FINALPROJECT_WORLD_H
