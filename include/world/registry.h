/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_REGISTRY_H
#define FINALPROJECT_REGISTRY_H

#include <string>
#include <render/textures.h>

namespace fp {
    typedef unsigned char block_type;
}

namespace fp::registry {
    
    /**
     * Bit masked enum description how the block should be treated by the renderer
     */
    enum block_visibility {
        // has no transparency
        OPAQUE = 0,
        // opaque but texture has alpha which should be treated as 100% transparent
        // this allows us to insert it into the opaque blocks render but requires us to include its neighbour's mesh
        TRANSPARENT_TEXTURE = 1,
        // texture is partially see-through
        TRANSLUCENT = 2,
        // block should be treated as 100% transparent everywhere
        TRANSPARENT = 4,
    };
    
    struct block_properties {
        // how should we handle this block? Blocks with transparent textures can be added to OPAQUE blocks
        block_visibility visibility = OPAQUE;
        // does this block produce light?
        bool produces_light = false;
        // WebGL doesn't default to empty textures, use index 0 to store an empty texture
        std::string textureName = "Air";
        
    };
    
    constexpr block_type AIR = 0;
    constexpr block_type STONE = 1;
    
    void registerBlock(block_type id, block_properties properties);
    void registerTexture(texture::file_texture* texture);
    
    void textureInit();
    void setupTextureLoaderThreads(int count = 8);
    void generateTexturePalette();
    void cleanup();
    
    block_properties get(block_type id);
    unsigned int getTextureID();
    texture::texture_index getTextureIndex(const std::string& name);
    
    
    /**
     * Registers all the default blocks used by the engine
     */
    inline void registerDefaultBlocks(){
        registerBlock(AIR, {TRANSPARENT});
        registerBlock(STONE, {});
    }
    
    inline void registerDefaultTextures() {
        textureInit();
        
        registerTexture(new texture::file_texture{"assets/textures/1676004600027876.jpg", "Stone"});
        registerTexture(new texture::file_texture{"assets/textures/1668750351593692.jpg", "Dirt"});
        registerTexture(new texture::file_texture{"assets/textures/1638777414645.jpg", "Dolph"});
        registerTexture(new texture::file_texture{"assets/textures/1603423355849.jpg", "Sit"});
        registerTexture(new texture::file_texture{"assets/textures/1603422678312.jpg", "Loser"});
        registerTexture(new texture::file_texture{"assets/textures/1592244663459.png", "Frog"});
        registerTexture(new texture::file_texture{"assets/textures/1592234267606.png", "Explode"});
    
        setupTextureLoaderThreads();
        generateTexturePalette();
    }
    
}
#endif //FINALPROJECT_REGISTRY_H
