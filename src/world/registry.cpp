/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <world/registry.h>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>
#include <queue>

std::unordered_map<fp::block_type, fp::registry::block_properties> blocks;
fp::texture::palette base_palette;
std::queue<fp::texture::file_texture*>* texture_loader_queues;
std::thread** texture_loader_threads;
int thread_count = 0;

void fp::registry::registerBlock(fp::block_type id, fp::registry::block_properties properties) {
    blocks[id] = std::move(properties);
}

fp::registry::block_properties fp::registry::get(fp::block_type id) {
    return blocks.at(id);
}

void fp::registry::registerTexture(fp::texture::file_texture* texture) {
    base_palette.registerTexture(texture);
}

unsigned int fp::registry::getTextureID(const std::string& name) {
    return base_palette.getTextureID();
}

fp::texture::texture_index fp::registry::getTextureIndex(const std::string& name) {
    return base_palette.getTexture(name);
}

void fp::registry::generatePalette() {
    for (int i = 0; i < thread_count; i++) {
        texture_loader_threads[i]->join();
        delete texture_loader_threads[i];
    }
    BLT_INFO("Finished loading all textures!");
    delete[] texture_loader_queues;
    delete[] texture_loader_threads;
    base_palette.generateGLTexture();
    BLT_INFO("Palette generated!");
}

void fp::registry::setupThreads(int count) {
    BLT_DEBUG("Setting up texture loading threads (%d)", count);
    thread_count = count;
    texture_loader_queues = new std::queue<fp::texture::file_texture*>[count];
    texture_loader_threads = new std::thread*[count];
    
    for (int i = 0; i < count; i++){
        texture_loader_threads[i] = new std::thread([](int thread_id) -> void {
            while (!texture_loader_queues[thread_id].empty()) {
                auto& top = texture_loader_queues[thread_id].front();
    
                auto texture_size = std::stoi(fp::settings::get("TEXTURE_SIZE"));
                auto t = texture::file_texture::resize(texture::file_texture::load(top), texture_size, texture_size);
                
                texture_loader_queues[thread_id].pop();
            }
        }, i);
    }
}
