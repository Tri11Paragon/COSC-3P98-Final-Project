/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_STORAGE_H
#define FINALPROJECT_STORAGE_H

#include <blt/math/math.h>
#include <vector>
#include "blt/std/logging.h"
#include "blt/std/format.h"
#include <world/chunk/typedefs.h>
#include <world/registry.h>
#include <unordered_map>
#ifdef __EMSCRIPTEN__
#include <phmap.h>
#else
#include <spp.h>
#endif

// contains storage classes for block IDs inside chunks plus eventual lookup of block states

namespace fp {
    
    class block_storage {
        private:
            block_type* blocks;
        public:
            block_storage() {
                blocks = new unsigned char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
                for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++)
                    blocks[i] = fp::registry::AIR;
            }
            
            ~block_storage() {
                delete[] blocks;
            }
            
            [[nodiscard]] inline block_type get(const block_pos& pos) const {
                return blocks[pos.z * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.x];
            }
            
            [[nodiscard]] inline bool checkBlockVisibility(const block_pos& pos) const {
                // blocks outside the check are always not visible to us. This requires special logic
                if (pos.x < 0 || pos.x >= CHUNK_SIZE || pos.y < 0 || pos.y >= CHUNK_SIZE || pos.z < 0 || pos.z >= CHUNK_SIZE) {
                    return false;
                }
                return fp::registry::get(get(pos)).visibility > fp::registry::OPAQUE;
            }
            
            inline void set(const block_pos& pos, block_type blockID) {
                blocks[pos.z * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.x] = blockID;
            }
    };
    
    class mesh_storage {
        private:
            // spp doesn't support emscripten, but phmap does work
            // slightly more memory consumption but still much lower than std::unordered_map (plus much faster access)
#ifdef __EMSCRIPTEN__
            phmap::flat_hash_map<vertex, unsigned int, _static::vertex_hash, _static::vertex_equality> created_vertices_index;
#else
            spp::sparse_hash_map<vertex, unsigned int, _static::vertex_hash, _static::vertex_equality> created_vertices_index;
#endif
            std::vector<vertex> vertices;
            std::vector<unsigned int> indices;
        public:
            /**
             * since a chunk mesh contains all the faces for all the blocks inside the chunk
             * we can add the translated values of predefined "unit" faces. This is for the simple "fast" chunk mesh generator.
             * @param face the direction the face is facing to be added to the mesh.
             * @param pos position of the face
             */
            void addFace(face face, const block_pos& pos, unsigned char texture_index);
            
            inline std::vector<vertex>& getVertices() {
                return vertices;
            }
            inline std::vector<unsigned int>& getIndices() {
                return indices;
            }
    };
    
    namespace mesh {
    
    }
    
}
#endif //FINALPROJECT_STORAGE_H
