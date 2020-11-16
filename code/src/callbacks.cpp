#include "callbacks.h"

#include "Helpers.h"

#include <glm/gtx/string_cast.hpp> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

namespace CSGY6533 {

BaseState::BaseState(Geometry& geometry)
    : m_geometry {geometry} {}

BaseState::~BaseState() {}

void BaseState::windowSizeCb(int width, int height) {
    glViewport(0, 0, width, height);
}

void BaseState::mouseClickCb(int button, int action,
                             double xworld, double yworld) {}

void BaseState::keyboardCb(int key, int action) {}

void BaseState::mouseMoveCb(double xworld, double yworld) {}

InsertState::InsertState(Geometry& geometry)
    : BaseState(geometry) {
}

InsertState::~InsertState() {
}

void InsertState::keyboardCb(int key, int action) {
    if(GLFW_PRESS == action) {
        switch (key)
        {
            case  GLFW_KEY_1:
                m_geometry.addCube();
                break;
            case GLFW_KEY_2:
                m_geometry.addBunny();
                break;
            case  GLFW_KEY_3:
                m_geometry.addBumpyCube();
                break;
            default:
                break;
        }
    }
}

MoveState::MoveState(Geometry& geometry)
        : BaseState(geometry) {}

MoveState::~MoveState() {
    m_selected = -1;
}

void MoveState::mouseClickCb(int button, int action,
                             double xworld, double yworld)  {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action) {
            glm::vec3 e = {xworld, yworld, 1.f};
            glm::vec3 d = {0.f, 0.f, -1.f}; //TODO: camera
            for (int i = 0; i < m_geometry.size(); ++i) {
                if (m_geometry[i].intersectRay(e, d)) {
                    m_selected = i;
                    std::cout << "Select: " << m_selected << std::endl;
                    return;
                }
            }
        }
    }

}

void MoveState::mouseMoveCb(double xworld, double yworld) {
    // if (lbutton_down) {
    // }
}



void MoveState::keyboardCb(int key, int action) {
    if(GLFW_PRESS == action && m_selected != -1) {
        switch (key) {
            case  GLFW_KEY_W:
                m_geometry[m_selected].translate(0.f, 0.1f, 0.f);
                break;
            case  GLFW_KEY_S:
                m_geometry[m_selected].translate(0.f, -0.1f, 0.f);
                break;
            case  GLFW_KEY_A:
                m_geometry[m_selected].translate(-0.1f, 0.f, 0.f);
                break;
            case  GLFW_KEY_D:
                m_geometry[m_selected].translate(0.1f, 0.f, 0.f);
            // TODO: move via z
                break;
            case  GLFW_KEY_Q:
                m_geometry[m_selected].translate(0.f, 0.f, 0.1f);
                break;
            case  GLFW_KEY_E:
                m_geometry[m_selected].translate(0.f, 0.f, -0.1f);
                break;
            case  GLFW_KEY_F:
                m_geometry[m_selected].rotate(20.f, 0.f, 0.f);
                break;
            case  GLFW_KEY_G:
                m_geometry[m_selected].rotate(-20.f, 0.f, 0.f);
                break;
            case  GLFW_KEY_H:
                m_geometry[m_selected].rotate(0.f, 20.f, 0.f);
                break;
            case  GLFW_KEY_J:
                m_geometry[m_selected].rotate(0.f, -20.f, 0.f);
                break;
            case  GLFW_KEY_K:
                m_geometry[m_selected].rotate(0.f, 0.f, 20.f);
                break;
            case  GLFW_KEY_L:
                m_geometry[m_selected].rotate(0.f, 0.f, -20.f);
                break;
            case GLFW_KEY_N:
                m_geometry[m_selected].scale(.1f);
                break;
            case GLFW_KEY_M:
                m_geometry[m_selected].scale(-.1f);
                break;
            case GLFW_KEY_Z:
                m_geometry[m_selected].setDisplayMode(Object::MODE1);
                break;
            case GLFW_KEY_X:
                m_geometry[m_selected].setDisplayMode(Object::MODE2);
                break;
            case GLFW_KEY_C:
                m_geometry[m_selected].setDisplayMode(Object::MODE3);
                break;
            #define SET_OBJECT_COLOR(xx) \
                case GLFW_KEY_ ##xx :   \
                    m_geometry[m_selected].color(provided_color[xx - 1]); \
                    break;
            SET_OBJECT_COLOR(1)
            SET_OBJECT_COLOR(2)
            SET_OBJECT_COLOR(3)
            SET_OBJECT_COLOR(4)
            SET_OBJECT_COLOR(5)
            SET_OBJECT_COLOR(6)
            SET_OBJECT_COLOR(7)
            SET_OBJECT_COLOR(8)
            SET_OBJECT_COLOR(9)
            #undef SET_POINT_COLOR

            default:
                break;
        }
    }
}

// void MoveState::reset() {
// }

CameraState::CameraState(Geometry& geometry, ViewControl& view_control)
    : BaseState(geometry)
    , m_view_control {view_control} {}

CameraState::~CameraState() {}

void CameraState::keyboardCb(int key, int action) {
    if(GLFW_PRESS == action) {
        switch (key) {
            case GLFW_KEY_W:
                m_view_control.up(0.1);
                break;
            case GLFW_KEY_S:
                m_view_control.down(0.1);
                break;
            case GLFW_KEY_A:
                m_view_control.left(0.1);
                break;
            case GLFW_KEY_D:
                m_view_control.right(0.1);
                break;
            case GLFW_KEY_Q:
                m_view_control.forward(0.1);
                break;
            case GLFW_KEY_E:
                m_view_control.backward(0.1);
                break;
        }
    }
}

void CameraState::mouseClickCb(int button, int action,
                              double xworld, double yworld) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action) {
        }
    }
}

DeleteState::DeleteState(Geometry& geometry)
    : BaseState(geometry) {
}

DeleteState::~DeleteState() {}

void DeleteState::mouseClickCb(int button, int action,
                               double xworld, double yworld) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action) {
            glm::vec3 e = {xworld, yworld, 1.f};
            glm::vec3 d = {0.f, 0.f, -1.f}; // TODO: camera
            for (int i = 0; i < m_geometry.size(); ++i) {
                if (m_geometry[i].intersectRay(e, d)) {
                    m_geometry.deleteObject(i);
                    std::cout << "Delete: " << i << std::endl;
                    return;
                }
            }
        }
    }
}

Callbacks::Callbacks(Geometry& geometry, ViewControl& view_control)
    : m_geometry {geometry}
    , m_view_control {view_control}
    , m_mode {DEFAULT} {
    m_cur = BaseState::ptr(new BaseState(m_geometry));
}

void Callbacks::toDefaultMode() {
    if (m_mode == DEFAULT) return;
    m_cur.reset(new BaseState(m_geometry));
}

void Callbacks::toModeInsert() {
    if (m_mode == INSERT) return;
    m_cur.reset(new InsertState(m_geometry));
}

void Callbacks::toModeMove() {
    if (m_mode == MOVE) return;
    m_cur.reset(new MoveState(m_geometry));
}

void Callbacks::toModeCamera() {
    if (m_mode == CAMERA) return;
    m_cur.reset(new CameraState(m_geometry, m_view_control));
}

void Callbacks::toModeDelete() {
    if (m_mode == DELETE) return;
    m_cur.reset(new DeleteState(m_geometry));
}

void Callbacks::mouseClickCb(int button, int action, double xworld, double yworld) {
    m_cur->mouseClickCb(button, action, xworld, yworld);
}

void Callbacks::keyboardCb(int key, int action) {
    if(GLFW_PRESS == action) {
        switch (key)
        {
            case  GLFW_KEY_I:
                toModeInsert();
                break;
            case GLFW_KEY_O:
                toModeMove();
                break;
            case  GLFW_KEY_P:
                toModeDelete();
                break;
            case  GLFW_KEY_U:
                toModeCamera();
                break;
            default:
                break;
        }
    }
    m_cur->keyboardCb(key, action);
}

void Callbacks::windowSizeCb(int width, int height) {
    m_cur->windowSizeCb(width, height);
}

void Callbacks::mouseMoveCb(double xworld, double yworld) {
    m_cur->mouseMoveCb(xworld, yworld);
}

std::vector<glm::vec3> MoveState::provided_color = {
    {0.509f, 0.223f, 0.207f},
    {0.537f, 0.745f, 0.698f},
    {0.788f, 0.729f, 0.513f},
    {0.870f, 0.827f, 0.549f},
    {0.870f, 0.611f, 0.325f},
    {0.701f, 0.839f, 0.431f},
    {0.125f, 0.141f, 0.180f},
    {0.360f, 0.654f, 0.729f},
    {0.466f, 0.203f, 0.376f}
};

}