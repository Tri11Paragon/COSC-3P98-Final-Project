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
#include <condition_variable>

std::unordered_map<fp::block_type, fp::registry::block_properties> blocks;

fp::texture::palette* base_palette;

std::mutex palette_mutex {};
std::mutex main_mutex {};
std::mutex queue_mutex {};
std::mutex completion_mutex {};
std::condition_variable main_wait {};

volatile unsigned int threads_left = 0;

std::queue<fp::texture::file_texture*>* texture_queue;
std::thread** texture_loader_threads;

int thread_count = 0;

void fp::registry::registerBlock(fp::block_type id, fp::registry::block_properties properties) {
    blocks[id] = std::move(properties);
}

fp::registry::block_properties fp::registry::get(fp::block_type id) {
    return blocks.at(id);
}

void fp::registry::registerTexture(fp::texture::file_texture* texture) {
    texture_queue->push(texture);
    BLT_TRACE("Queued texture %s", texture->getName().c_str());
}

unsigned int fp::registry::getTextureID() {
    return base_palette->getTextureID();
}

fp::texture::texture_index fp::registry::getTextureIndex(const std::string& name) {
    return base_palette->getTexture(name);
}

void fp::registry::generateTexturePalette() {
    for (int i = 0; i < thread_count; i++) {
        texture_loader_threads[i]->join();
        delete texture_loader_threads[i];
    }
    BLT_INFO("Finished loading all textures!");
    delete texture_queue;
    delete[] texture_loader_threads;
    base_palette->generateGLTexture();
    BLT_INFO("Palette generated!");
}

void fp::registry::setupTextureLoaderThreads(int count) {
    BLT_DEBUG("Setting up texture loading threads (%d)", count);
    thread_count = count;
    texture_loader_threads = new std::thread*[count];
    
    threads_left = count;
    
    for (int i = 0; i < count; i++){
        texture_loader_threads[i] = new std::thread([](int thread_id) -> void {
            while (!texture_queue->empty()) {
                queue_mutex.lock();
                auto& top = texture_queue->front();
                texture_queue->pop();
                queue_mutex.unlock();
    
                auto texture_size = std::stoi(fp::settings::get("TEXTURE_SIZE"));
                auto t = texture::file_texture::resize(texture::file_texture::load(top), texture_size, texture_size);
                
                std::scoped_lock<std::mutex> lock(palette_mutex);
                base_palette->registerTexture(t);
                BLT_TRACE("Loaded file %s", t->getName().c_str());
            }
            std::scoped_lock<std::mutex> lock{completion_mutex};
            threads_left--;
            BLT_TRACE("Loading thread finished %d more to go", threads_left);
            main_wait.notify_one();
        }, i);
    }
    
    std::unique_lock<std::mutex> main_lock{main_mutex};
    main_wait.wait(main_lock, []() -> bool { return threads_left <= 0; });
}

void fp::registry::textureInit() {
    texture_queue = new std::queue<fp::texture::file_texture*>();
    base_palette = new texture::palette();
}

void fp::registry::cleanup() {
    delete base_palette;
}
