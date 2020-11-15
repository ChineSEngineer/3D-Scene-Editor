#ifndef __VIEW_CONTROL_H__
#define __VIEW_CONTROL_H__

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3

namespace CSGY6533 {

class ViewControl {
 public:
    ViewControl();
    glm::mat4 getViewMatrix();
    glm::mat4 getOrthoProjMatrix();
    glm::mat4 getAspectRatioMatrix();

    glm::vec2 worldCoordinateFromView(float x, float y);

    void left(float lengh);
    void right(float length);
    void up(float lengh);
    void down(float lengh);
    void forward(float length);
    void backward(float length);

    void setScreenSize(int height, int width);
    glm::vec3 getEyePosition() { return m_eyePos; }


 private:
    class Trackball {
     public:
        void left(float length);
        void right(float length);
        void up(float length);
        void down(float length);
        void forward(float length);
        void backward(float length);
        glm::vec3 getEyePos() const;
        
     private:
        static float d2r(float d);
        float lengthToDegree(float length);
     private:
        float m_theta = 90.f;
        float m_phi = 0.f;
        float m_radius = 3.f;
    };
    float m_l;
    float m_r;
    float m_b;
    float m_t;
    float m_n;
    float m_f;

    float m_height;
    float m_width;

    glm::vec3 m_eyePos;
    glm::vec3 m_viewUp;
    Trackball m_trackball;
};  
       
}

#endif // __VIEW_CONTROL_H__

