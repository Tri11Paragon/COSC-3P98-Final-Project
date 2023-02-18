/*
 * Created by Brett on 17/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_SETTINGS_H
#define FINALPROJECT_SETTINGS_H

#include <string>

namespace fp::settings {

    void load(const std::string& file);
    void save(const std::string& file);
    
    std::string get(const std::string& property);
    void set(const std::string& property, const std::string& value);

}

#endif //FINALPROJECT_SETTINGS_H
