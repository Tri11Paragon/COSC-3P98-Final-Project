/*
 * Created by Brett on 10/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

// internal types used by graphics to prevent cluttering of the main graphics.cpp file. Do not include elsewhere.
#ifndef FINALPROJECT_PLANES_H
#define FINALPROJECT_PLANES_H

#include <blt/math/vectors.h>
#include <blt/math/matrix.h>
#include <blt/std/logging.h>
#include <blt/math/log_util.h>
#include <memory>
#include <array>

namespace fp::graphics {
    
    typedef struct {
        float x,y,z;
    } plane_vertex;
    
    inline plane_vertex blt_toVertex(const blt::vec3& v){
        return plane_vertex{v.x(), v.y(), v.z()};
    }
    
    struct plane_model {
        std::array<plane_vertex, 4> vertices;
        std::array<unsigned int, 6> indices;
        
        plane_model(
                const std::array<plane_vertex, 4>& vertices, const std::array<unsigned int, 6>& indices
        ): vertices(vertices), indices(indices) {}
    };
    
    struct plane {
        blt::vec4 plane;
        blt::vec3 color;
    };
    
    static std::unique_ptr<plane_model> createFlatPlane(const blt::vec4& plane, float scale) {
        // x + y + z + d = 0
        // 0 + n * y + 0 + d = 0
        // y = -d / n
        //blt::vec3 pointOnPlane{0, -plane.w() / plane.y(), 0};
        // x + y + z + d = 0
        // 0 + 0 + n * z + d = 0
        // z = -d / n
        //blt::vec3 other_pointOnPlane{0, 0, -plane.w() / plane.z()};
        
        //blt::vec3 vecOnPlane = (pointOnPlane - other_pointOnPlane).normalize();
        blt::vec3 normal {plane.x(), plane.y(), plane.z()};
        blt::vec3 tangent {};
        blt::vec3 bitangent {};
        
        blt::vec_algorithm::findOrthogonalBasis(normal, normal, tangent, bitangent);
        
        // project the tangent
        //blt::vec3 tangent = (vecOnPlane - normal * blt::vec3::dot(normal, vecOnPlane)).normalize();
        
        // construct the bitangnet
        //blt::vec3 bitangent = blt::vec3::cross(normal, tangent).normalize();
        
        // the tangent and the bitangent can be used to construct the plane vertices
        std::array<plane_vertex, 4> vertices = {
                blt_toVertex(-tangent * scale - bitangent * scale),
                blt_toVertex(tangent * scale - bitangent * scale),
                blt_toVertex(tangent * scale + bitangent * scale),
                blt_toVertex(-tangent * scale + bitangent * scale)
        };
        // x,   y,      z,
//                blt::vec3{scale, 0, scale},  // top vecOnPlane
//                blt::vec3{-scale, 0, scale},  // bottom vecOnPlane
//                blt::vec3{-scale, 0, -scale},  // bottom left
//                blt::vec3{scale, 0, -scale},   // top left
        
        std::array<unsigned int, 6> indices = {
                3, 1, 0,
                3, 2, 1
        };
        return std::make_unique<plane_model>(vertices, indices);
    }
}

#endif //FINALPROJECT_PLANES_H
