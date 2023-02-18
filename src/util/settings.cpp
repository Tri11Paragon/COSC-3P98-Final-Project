/*
 * Created by Brett on 17/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <util/settings.h>
#include <blt/std/loader.h>
#include <blt/std/string.h>
#include <blt/std/logging.h>
#include <unordered_map>

std::unordered_map<std::string, std::string> properties;

// will be overwritten if the file has something different.
void write_default_values(){
    properties["TEXTURE_SIZE"] = std::to_string(32);
}

void fp::settings::load(const std::string& file) {
    write_default_values();
    
    BLT_INFO("Loading settings file %s!", file.c_str());
    
    try {
        auto lines = blt::fs::getLinesFromFile(file);
        for (const auto& line : lines) {
            auto split_line = blt::string::split(line, "=");
        
            if (split_line.size() < 2) {
                BLT_WARN("Unable to load line '%s' due to incomplete property (property = value)");
                continue;
            }
        
            auto& property = blt::string::trim(split_line[0]);
            auto& value = blt::string::trim(split_line[1]);
        
            properties[property] = value;
        }
    } catch (std::exception& e) {}
}

void fp::settings::save(const std::string& file) {
    std::ofstream output {file};
    output.exceptions(std::ios::failbit | std::ios::badbit);
    for (const auto& property : properties){
        output << property.first << " = " << property.second;
    }
}

std::string fp::settings::get(const std::string& property) {
    return properties.at(property);
}

void fp::settings::set(const std::string& property, const std::string& value) {
    properties[property] = value;
}
