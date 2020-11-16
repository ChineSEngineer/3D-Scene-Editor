#include "view_control.h"

#include "macro.h"

#include <glm/gtx/string_cast.hpp> 

#include <iostream>
#include <cmath>

namespace CSGY6533 {

ViewControl::ViewControl() 
    : m_l {-1.f}, m_r {1.f}
    , m_b {-1.f}, m_t {1.f}
    , m_n {0.1f}, m_f {100.f}
    , m_fov {40.f}
    , m_viewUp {0.f, 1.f, 0.f}
    , m_trackball {}
    , m_project_mode {ORTHOGRAPHIC} {
        m_eyePos = m_trackball.getEyePos();
    }

glm::mat4 ViewControl::getViewMatrix() {
    glm::mat4 view = glm::lookAt(m_eyePos, glm::vec3(0.f), m_viewUp);
    
    return view;
}

glm::mat4 ViewControl::getOrthoProjMatrix() {
    return glm::ortho(m_l, m_r, m_l, m_r, m_n ,m_f);
}

glm::mat4 ViewControl::getProjMatrix() {
    if (m_project_mode == PERSPECTIVE) {
        return getPerspProjMatrix();
    } else if (m_project_mode == ORTHOGRAPHIC) {
        return getOrthoProjMatrix();
    } else {
        ASSERT(false, "Invalid projection mode");
    }
}

glm::mat4 ViewControl::getPerspProjMatrix() {
    return glm::perspective(glm::radians(m_fov), 1.f, m_n, m_f);
}

glm::mat4 ViewControl::getAspectRatioMatrix() {
    return glm::scale(glm::mat4(1.f), glm::vec3(static_cast<float>(m_height) / static_cast<float>(m_width), 1.f, 1.f));
}

glm::vec2 ViewControl::worldCoordinateFromView(float x, float y) {
    glm::vec4 world = glm::inverse(getViewMatrix()) * glm::inverse(getOrthoProjMatrix()) * glm::inverse(getViewMatrix()) * glm::vec4(x, y, 0.f, 1.f);
    return {world[0], world[1]};
}

std::pair<glm::vec3, glm::vec3> ViewControl::getClickRay(float x, float y) {
    glm::mat4 convert = glm::inverse(getProjMatrix() * getViewMatrix());

    glm::vec4 start = convert * glm::vec4(x, y, -1.f, 1.f);
    glm::vec4 end   = convert * glm::vec4(x, y, 1.f, 1.f);
    start /= start[3];
    end   /= end[3];
    // std::cout << "start=" << glm::to_string(start) << ", end=" << glm::to_string(end) << std::endl;
    return {glm::vec3(start), glm::normalize(glm::vec3(end - start))};
}


void ViewControl::left(float length) {
    m_trackball.left(length);
    m_eyePos = m_trackball.getEyePos();
    // right(-length);
}

void ViewControl::right(float length) {
    m_trackball.right(length);
    m_eyePos = m_trackball.getEyePos();
    // m_eyePos[0] += length;
}

void ViewControl::up(float length) {
    m_trackball.up(length);
    m_eyePos = m_trackball.getEyePos();
    // m_eyePos[1] += length;
}

void ViewControl::down(float length) {
    m_trackball.down(length);
    m_eyePos = m_trackball.getEyePos();
    // up(-length);
}

void ViewControl::forward(float length) {
    m_trackball.forward(length);
    m_eyePos = m_trackball.getEyePos();
    // m_eyePos[2] += length;
}

void ViewControl::backward(float length) {
    m_trackball.backward(length);
    m_eyePos = m_trackball.getEyePos();
    // forward(-length);
}

void ViewControl::setScreenSize(int height, int width) {
    m_height = height;
    m_width = width;
}


void ViewControl::Trackball::left(float length) {
    m_phi = std::fmod(m_phi - lengthToDegree(length) + 360, 360.f);
}

void ViewControl::Trackball::right(float length) {
    m_phi = std::fmod(m_phi + lengthToDegree(length), 360.f);
}

void ViewControl::Trackball::up(float length) {
    float tmp = m_theta - lengthToDegree(length);
    if (tmp > 0) {
        m_theta = tmp;
    }
}

void ViewControl::Trackball::down(float length) {
    float tmp = m_theta + lengthToDegree(length);
    if (tmp < 180) {
        m_theta = tmp;
    }
}

void ViewControl::Trackball::forward(float length) {
    if (m_radius - length > 0) {
        m_radius -= length;
    }
}

void ViewControl::Trackball::backward(float length) { m_radius += length; }

glm::vec3 ViewControl::Trackball::getEyePos() const {
    float phi = d2r(m_phi);
    float theta = d2r(m_theta);
    float x = m_radius * sin(theta) * sin(phi);
    float y = m_radius * cos(theta);
    float z = m_radius * sin(theta) * cos(phi);
    // std::cout << "r=" << m_radius << ", theta=" << m_theta << ", phi=" << m_phi << std::endl;;
    return {x, y, z};
}

float ViewControl::Trackball::d2r(float d) { return (d / 180.f) * ((float) M_PI); }

float ViewControl::Trackball::lengthToDegree(float length) {
    return std::fmod(360 * (length / (2 * ((float) M_PI) * m_radius)), 360);
}

};  // CSGY6533
