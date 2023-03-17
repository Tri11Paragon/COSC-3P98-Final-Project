/*
 * Created by Brett on 13/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_CHUNK_TYPEDEFS_H
#define FINALPROJECT_CHUNK_TYPEDEFS_H

// size of the chunk in number of blocks
constexpr int CHUNK_SIZE = 32;
const int CHUNK_SHIFT = (int) (log(CHUNK_SIZE) / log(2));
// size that the base vertex arrays are assumed to be (per face)
constexpr int VTX_ARR_SIZE = 4;

namespace fp {
    
    enum face {
        X_POS = 0,
        X_NEG = 1,
        Y_POS = 2,
        Y_NEG = 3,
        Z_POS = 4,
        Z_NEG = 5,
    };
    
    enum chunk_mesh_status {
        // chunk is okay and needs no special action
        OKAY = 0,
        // chunk needs its VAO updated with the newest mesh
        REFRESH = 1,
        // chunk needs a complete re-mesh.
        DIRTY = 2
    };
    
    enum chunk_update_status {
        NONE = 0,
        NEIGHBOUR_CREATE = 1,
    };
    
    struct chunk_pos {
        int x, y, z;
    };
    
    struct block_pos {
        int x, y, z;
        
        block_pos(int x, int y, int z): x(x), y(y), z(z) {}
        
        block_pos(float x, float y, float z): block_pos(int(x), int(y), int(z)) {}
    };
    
    // to ensure this is a POD we define the vertex as a C-struct. This allows us to store one large vertex array and pass that to the GPU
    // instead of sending arrays for the positions, UVs, normals, etc.
    // since OpenGL allows us to specify attributes based on offsets from the same VBO.
    typedef struct {
        // since opaque textures only use 4 possible coords in our basic engine
        // UVs can be stored on the gpu as a const array, using 2 bits we can index into them
        // texture arrays store 256 max possible textures, so 1 byte can store that.
        // position can be stored using 33 values or 6 bits each, taking 18 bits total.
        // leaving us with 4 bits currently unused in the float.
        float data;
    } vertex;
    
    typedef struct {
        float x, y, z;
        float u, v;
        float texture_index;
    } unpacked_vertex;
    
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
        
        struct vertex_hash {
            inline size_t operator()(const vertex& pos) const {
//                size_t p1 = std::hash<float>()(pos.x);
//                size_t p2 = std::hash<float>()(pos.y);
//                size_t p3 = std::hash<float>()(pos.z);
//                return (p1 ^ (p2 << 1)) ^ p3;
                return std::hash<float>()(pos.data);
            }
        };
        
        struct chunk_pos_equality {
            inline bool operator()(const chunk_pos& p1, const chunk_pos& p2) const {
                return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
            }
        };
        
        struct vertex_equality {
            inline bool operator()(vertex p1, vertex p2) const {
//                return f_equal(p1.x, p2.x) && f_equal(p1.y, p2.y) && f_equal(p1.z, p2.z) &&
//                       f_equal(p1.u, p2.u) && f_equal(p1.v, p2.v) && p1.index == p2.index;
                // comparison is now only a single value that is directly comparable! (since data is stored at the bit level)
                return p1.data == p2.data;
            }
        };
        
    }
}
#endif //FINALPROJECT_CHUNK_TYPEDEFS_H
