/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_STORAGE_H
#define FINALPROJECT_STORAGE_H

#include <blt/std/math.h>
#include <vector>
#include "blt/std/logging.h"
#include <world/chunk/typedefs.h>

// contains storage classes for block IDs inside chunks plus eventual lookup of block states

namespace fp {
    
    class block_storage {
        private:
            char* blocks;
        public:
            block_storage() {
                blocks = new char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
                for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++)
                    blocks[i] = 0;
            }
            
            ~block_storage() {
                delete[] blocks;
            }
            
            [[nodiscard]] inline char get(const block_pos& pos) const {
                return blocks[pos.z * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.x];
            }
            
            inline void set(const block_pos& pos, char blockID) {
                blocks[pos.z * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.x] = blockID;
            }
    };
    
    class mesh_storage {
        private:
            std::vector<float> vertices;
            inline void add_and_translate(const float* array, const block_pos& pos) {
                // since a chunk mesh contains all the faces for all the blocks inside the chunk
                // we can add the translated values of predefined "unit" faces. This is for the simple "fast" chunk mesh generator.
                for (int i = 0; i < VTX_ARR_SIZE; i+=3){
                    auto new_x = array[i] + (float)pos.x;
                    auto new_y = array[i + 1] + (float)pos.y;
                    auto new_z = array[i + 2] + (float)pos.z;
                    BLT_TRACE("Creating translated vertex {%f, %f, %f} from array position [%d, %d, %d]", new_x, new_y, new_z, i, i + 1, i + 2);
                    vertices.push_back(new_x);
                    vertices.push_back(new_y);
                    vertices.push_back(new_z);
                }
            }
        public:
            void addFace(face face, const block_pos& pos);
            
            inline std::vector<float>& getVertices() {
                return vertices;
            }
    };
    
    namespace mesh {
    
    }
    
}
#endif //FINALPROJECT_STORAGE_H
