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

// contains storage classes for block IDs inside chunks plus eventual lookup of block states

// size of the chunk in number of blocks
constexpr int CHUNK_SIZE = 32;
const int CHUNK_SHIFT = (int)(log(CHUNK_SIZE) / log(2));
// size that the base vertex arrays are assumed to be (per face)
constexpr int VTX_ARR_SIZE = 12;

namespace fp {
    
    enum face {
        X_POS = 0,
        X_NEG = 1,
        Y_POS = 2,
        Y_NEG = 3,
        Z_POS = 4,
        Z_NEG = 5,
    };
    
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
            
            inline char get(const blt::vec3& pos) { return get(pos.x(), pos.y(), pos.z()); }
            
            inline char get(float x, float y, float z) { return get(int(x), int(y), int(z)); }
            
            inline char get(int x, int y, int z) {
                return blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x];
            }
            
            inline void set(const blt::vec3& pos, char blockID) { return set(pos.x(), pos.y(), pos.z(), blockID); }
            
            inline void set(float x, float y, float z, char blockID) { return set(int(x), int(y), int(z), blockID); }
            
            inline void set(int x, int y, int z, char blockID) {
                blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x] = blockID;
            }
    };
    
    class mesh_storage {
        private:
            std::vector<float> vertices;
            inline void add_and_translate(const float* array, float x, float y, float z) {
                // since a chunk mesh contains all the faces for all the blocks inside the chunk
                // we can add the translated values of predefined "unit" faces. This is for the simple "fast" chunk mesh generator.
                for (int i = 0; i < VTX_ARR_SIZE; i+=3){
                    auto new_x = array[i] + x;
                    auto new_y = array[i + 1] + y;
                    auto new_z = array[i + 2] + z;
                    BLT_TRACE("Creating translated vertex {%f, %f, %f} from array position [%d, %d, %d]", new_x, new_y, new_z, i, i + 1, i + 2);
                    vertices.push_back(new_x);
                    vertices.push_back(new_y);
                    vertices.push_back(new_z);
                }
            }
        public:
            void addFace(face face, float x, float y, float z);
            inline void addFace(face face, int x, int y, int z) {
                addFace(face, (float)x, (float)y, (float)z);
            }
            
            inline std::vector<float>& getVertices() {
                return vertices;
            }
    };
    
    namespace mesh {
    
    }
    
}
#endif //FINALPROJECT_STORAGE_H
