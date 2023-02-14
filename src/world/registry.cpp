/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/registry.h>
#include <unordered_map>

std::unordered_map<fp::block_type, fp::registry::block_properties> blocks;

void fp::registry::registerBlock(fp::block_type id, fp::registry::block_properties properties) {
    blocks[id] = properties;
}

fp::registry::block_properties fp::registry::get(fp::block_type id) {
    return blocks.at(id);
}
