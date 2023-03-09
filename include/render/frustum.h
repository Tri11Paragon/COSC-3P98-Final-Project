/*
 * Created by Brett on 08/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_FRUSTUM_H
#define FINALPROJECT_FRUSTUM_H

#include <blt/math/math.h>
#include <render/gl.h>

// based on
// http://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/

class frustum {
    private:
        class plane {
            private:
                float m_A = 0, m_B = 0, m_C = 0, m_D = 0;
            public:
                plane() = default;
                plane(float A, float B, float C, float D): m_A(A), m_B(B), m_C(C), m_D(D) {
                    normalize();
                }
                
                inline void normalize(){
                    blt::vec3 normal{m_A, m_B, m_C};
                    auto mag = normal.magnitude();
                    m_A = m_A / mag;
                    m_B = m_B / mag;
                    m_C = m_C / mag;
                    m_D = m_D / mag;
                }
                
                [[nodiscard]] inline float distance(const blt::vec3& point) const {
                    return m_A * point.x() + m_B * point.y() + m_C * point.z() + m_D;
                }
                
                [[nodiscard]] inline float A() const { return m_A; }
                
                [[nodiscard]] inline float B() const { return m_B; }
                
                [[nodiscard]] inline float C() const { return m_C; }
                
                [[nodiscard]] inline float D() const { return m_D; }
        };
        
        enum SIDE {
            LEFT = 0,
            RIGHT = 1,
            TOP = 2,
            BOTTOM = 3,
            NEAR = 4,
            FAR = 5,
        };
        
        blt::mat4x4 PM;
        plane planes[6];
    public:
        frustum() = default;
        
        void update() {
            PM = fp::getProjectionMatrix() * fp::getViewMatrix();
            
            planes[LEFT] = plane{
                    PM.m(0, 0) + PM.m(3, 0),
                    PM.m(0, 1) + PM.m(3, 1),
                    PM.m(0, 2) + PM.m(3, 2),
                    PM.m(0, 3) + PM.m(3, 3)
            };
            
            planes[RIGHT] = plane{
                    -PM.m(0, 0) + PM.m(3, 0),
                    -PM.m(0, 1) + PM.m(3, 1),
                    -PM.m(0, 2) + PM.m(3, 2),
                    -PM.m(0, 3) + PM.m(3, 3)
            };
            
            planes[BOTTOM] = plane{
                    PM.m(1, 0) + PM.m(3, 0),
                    PM.m(1, 1) + PM.m(3, 1),
                    PM.m(1, 2) + PM.m(3, 2),
                    PM.m(1, 3) + PM.m(3, 3)
            };
            
            planes[TOP] = plane{
                    -PM.m(1, 0) + PM.m(3, 0),
                    -PM.m(1, 1) + PM.m(3, 1),
                    -PM.m(1, 2) + PM.m(3, 2),
                    -PM.m(1, 3) + PM.m(3, 3)
            };
            
            planes[NEAR] = plane{
                    PM.m(2, 0) + PM.m(3, 0),
                    PM.m(2, 1) + PM.m(3, 1),
                    PM.m(2, 2) + PM.m(3, 2),
                    PM.m(2, 3) + PM.m(3, 3)
            };
    
            planes[FAR] = plane{
                    -PM.m(2, 0) + PM.m(3, 0),
                    -PM.m(2, 1) + PM.m(3, 1),
                    -PM.m(2, 2) + PM.m(3, 2),
                    -PM.m(2, 3) + PM.m(3, 3)
            };
            
        }
        
        bool pointInside(const blt::vec3& point){
            // std::all_of ??
            for (auto plane : planes){
                // point is outside plane
                if (plane.distance(point) < 0)
                    return false;
            }
            return true;
        }
        
        bool cubeInside(const blt::vec3& start, const blt::vec3& end) {
            return pointInside(start) || pointInside(end);
        }
};

#endif //FINALPROJECT_FRUSTUM_H
