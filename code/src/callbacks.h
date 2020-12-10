#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include "geometry.h"

#include <memory>

namespace CSGY6533 {

class BaseState {
 public:
    typedef std::unique_ptr<BaseState> ptr;
    BaseState(Geometry& geometry, ViewControl& view_control);
    virtual ~BaseState();
    virtual void windowSizeCb(int width, int height);
    virtual void mouseClickCb(int button, int action,
                              double xworld, double yworld);
    virtual void keyboardCb(int key, int action);
    virtual void mouseMoveCb(double xworld, double yworld);
 protected:
    Geometry& m_geometry;
    ViewControl& m_view_control;
};

class InsertState : public BaseState {
 public:
    InsertState(Geometry& geometry, ViewControl& view_control);
    virtual ~InsertState();
    void keyboardCb(int key, int action);
};

class MoveState : public BaseState {
 public:
    MoveState(Geometry& geometry, ViewControl& view_control);
    virtual ~MoveState();
    void mouseClickCb(int button, int action,
                      double xworld, double yworld) override;
    void mouseMoveCb(double xworld, double yworld) override;
    void keyboardCb(int key, int action) override;
 // private:
 //    void reset();
 private:
 // bool lbutton_down = false;
    int m_selected = -1;
    static std::vector<glm::vec3> provided_color;
 //    glm::vec2 m_previous;
 //    float m_x = 0.f;
 //    float m_y = 0.f;
};

class CameraState : public BaseState {
 public:
    CameraState(Geometry& geometry, ViewControl& view_control);
    virtual ~CameraState();
    void mouseClickCb(int button, int action,
                      double xworld, double yworld) override;
    void keyboardCb(int key, int action) override;
 private:
 //    int m_selected = -1;
};

class DeleteState : public BaseState {
 public:
    DeleteState(Geometry& geometry, ViewControl& view_control);
    virtual ~DeleteState();
    void mouseClickCb(int button, int action,
                      double xworld, double yworld) override;
};

class LightState : public BaseState {
 public:
    LightState(Geometry& geometry, ViewControl& view_control);
    virtual ~LightState();
    void keyboardCb(int key, int action) override;
};

class Callbacks {
 public:
    Callbacks(Geometry& geometry, ViewControl& m_view_control);

    void toDefaultMode();
    void toModeInsert();
    void toModeMove();
    void toModeCamera();
    void toModeDelete();
    void toModeLight();
    void mouseClickCb(int button, int action, double xworld, double yworld);
    void keyboardCb(int key, int action);
    void windowSizeCb(int width, int height);
    void mouseMoveCb(double xworld, double yworld);
 private:
    enum Mode {
        DEFAULT = 0,
        INSERT = 1,
        MOVE = 2,
        CAMERA = 3,
        DELETE = 4,
        LIGHT = 5
    };
    BaseState::ptr m_cur; 
    Geometry& m_geometry;
    ViewControl& m_view_control;
    Mode m_mode;
};

}

#endif // __CALLBACKS_H__
