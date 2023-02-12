/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_CHUNK_H
#define FINALPROJECT_CHUNK_H

#include <world/chunk/storage.h>
#include <render/gl.h>

namespace fp {
    
    class chunk {
        private:
            block_storage* storage;
            VAO* chunk_vao;
            
            bool isDirty = false;
            unsigned long render_size = 0;
        protected:
            mesh_storage* generateMesh();
        
            /**
             * Converts from world coord to chunk coords
             * @param coord world space coordinate
             * @return chunk internal coord
             */
            static inline int cc(int coord) {
                auto val = coord % CHUNK_SIZE;
                return val < 0 ? CHUNK_SIZE + val : val;
            }
            
        public:
            chunk() {
                storage = new block_storage();
                chunk_vao = new VAO();
                // set up the VBOs which will be later updated when the mesh is generated.
                chunk_vao->bindVBO(new VBO(ARRAY_BUFFER, nullptr, 0), 0, 3);
                chunk_vao->bindElementVBO(new VBO(ELEMENT_BUFFER, nullptr, 0));
            }
            
            void update();
            void render(fp::shader& chunk_shader);
            
            inline void setBlock(int x, int y, int z, char blockID){
                // mark the chunk for a mesh update
                isDirty = true;
                storage->set(cc(x), cc(y), cc(z), blockID);
            }
            
            inline void setBlock(float x, float y, float z, char blockID){setBlock((int)x,(int)y,(int)z, blockID);}
            
            inline char getBlock(int x, int y, int z){
                return storage->get(x, y, z);
            }
            
            inline char getBlock(float x, float y, float z){
                return getBlock((int)x,(int)y,(int)z);
            }
            
            ~chunk() {
                delete storage;
            }
    };
    
}

#endif //FINALPROJECT_CHUNK_H
