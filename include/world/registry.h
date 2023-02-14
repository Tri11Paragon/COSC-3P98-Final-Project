/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_REGISTRY_H
#define FINALPROJECT_REGISTRY_H

namespace fp {
    typedef unsigned char block_type;
}

namespace fp::registry {
    
    enum block_visibility {
        OPAQUE = 0,
        TRANSLUCENT = 1,
        TRANSPARENT = 2,
    };
    
    struct block_properties {
        block_visibility visibility = OPAQUE;
    };
    
    constexpr block_type AIR = 0;
    constexpr block_type STONE = 1;
    
    void registerBlock(block_type id, block_properties properties);
    
    block_properties get(block_type id);
    
    
    /**
     * Registers all the default blocks used by the engine
     */
    inline void registerDefaultBlocks(){
        registerBlock(AIR, {TRANSPARENT});
        registerBlock(STONE, {});
    }
    
}
#endif //FINALPROJECT_REGISTRY_H
