/*
 * Created by Brett on 13/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_CHUNK_TYPEDEFS_H
#define FINALPROJECT_CHUNK_TYPEDEFS_H

// size of the chunk in number of blocks
constexpr int CHUNK_SIZE = 32;
const int CHUNK_SHIFT = (int)(log(CHUNK_SIZE) / log(2));
// size that the base vertex arrays are assumed to be (per face)
constexpr int VTX_ARR_SIZE = 18;

namespace fp {
    
    enum face {
        X_POS = 0,
        X_NEG = 1,
        Y_POS = 2,
        Y_NEG = 3,
        Z_POS = 4,
        Z_NEG = 5,
    };
    
    enum chunk_status {
        // chunk is okay and needs no special action
        OKAY = 0,
        // chunk needs its VAO updated with the newest mesh
        REFRESH = 1,
        // chunk needs its edges re-meshed because it was created with null neighbours
        PARTIAL_MESH = 2,
        // chunk needs a complete re-mesh.
        FULL_MESH = 3
    };
    
    struct chunk_pos {
        int x, y, z;
    };
    
    struct block_pos {
        int x, y, z;
        
        block_pos(int x, int y, int z): x(x), y(y), z(z) {}
        
        block_pos(float x, float y, float z): block_pos(int(x), int(y), int(z)) {}
    };
    
    namespace _static {
    
        // std::unordered_map requires a type. As a result the functions are encapsulated.
        struct chunk_pos_hash {
            inline size_t operator()(const chunk_pos& pos) const {
                size_t p1 = std::hash<int>()(pos.x);
                size_t p2 = std::hash<int>()(pos.y);
                size_t p3 = std::hash<int>()(pos.z);
                return (p1 ^ (p2 << 1)) ^ p3;
            }
        };
    
        struct chunk_pos_equality {
            inline bool operator()(const chunk_pos& p1, const chunk_pos& p2) const {
                return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
            }
        };
        
    }
}
#endif //FINALPROJECT_CHUNK_TYPEDEFS_H
