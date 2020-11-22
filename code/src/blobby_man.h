#ifndef __BLOBBY_MAN_H__
#define __BLOBBY_MAN_H__

#include "Helpers.h"
#include "matrix.h"

#include <glm/glm.hpp> // glm::vec3
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

namespace CSGY6533 {


class BlobbyMan {
 public:
    enum BodyPart {
        HEAD = 0,
        BODY = 1,
        LEFTARM = 2,
        RIGHTARM = 3,
        LEFTLEG = 4,
        RIGHTLEG = 5,
        CAMERA = 6
    };
    enum MoveDir{
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        FORWARD = 4,
        BACKWARD = 5
    };

    void resize_window(int width, int height);

    static void degree_add(float& a, float b) {
        if (a + b < 90.f && a + b > -90.f)  a += b;
    }

    static void degree_add2(float& a, float b) {
        if (a + b < 90.f && a + b > 0.f)  a += b;
    }

    static void degree_add3(float& a, float b) {
        if (a + b < 0.f && a + b > -90.f)  a += b;
    }

    static void degree_add4(float& a, float b) {
        if (a + b < 180.f && a + b > -180.f)  a += b;
        else if (a + b > 180.f) a = (a + b) - 360.f;
        else if (a + b < -180.f) a = (a + b) + 360.f;
    }

    static void degree_add5(float& a, float b) {
        if (a + b < 160.f && a + b > 0.f)  a += b;
    }

    static void degree_add6(float& a, float b) {
        if (a + b < 0.f && a + b > -160.f)  a += b;
    }

    static void degree_add7(float& a, float b) {
        if (a + b < 45.f && a + b > -45.f)  a += b;
    }

    void head_move(MoveDir dir, float angle);
    void camera_move(MoveDir dir, float angle);
    void body_move(MoveDir dir, float angle);
    void left_leg_move(MoveDir dir, float angle);
    void right_leg_move(MoveDir dir, float angle);
    void left_arm_move(MoveDir dir, float angle);
    void right_arm_move(MoveDir dir, float angle);
    void start() {
        last_update = std::chrono::high_resolution_clock::now();
        start_flag = true;
    }
    void stop() {
        start_flag = false;
    }
    void update_color();
    void update();
    void keyboardCb(int key, int action) {
        if(GLFW_PRESS == action) {
            if (key == GLFW_KEY_W ||
                key == GLFW_KEY_S ||
                key == GLFW_KEY_A ||
                key == GLFW_KEY_D ||
                key == GLFW_KEY_Q ||
                key == GLFW_KEY_E) {
                start();
            } else {
                stop();
            }
            switch (key) {
                case GLFW_KEY_0:
                    selected = CAMERA;
                    break;
                case GLFW_KEY_1:
                    selected = HEAD;
                    break;
                case GLFW_KEY_2:
                    selected = BODY;
                    break;
                case GLFW_KEY_3:
                    selected = LEFTARM;
                    break;
                case GLFW_KEY_4:
                    selected = RIGHTARM;
                    break;
                case GLFW_KEY_5:
                    selected = LEFTLEG;
                    break;
                case GLFW_KEY_6:
                    selected = RIGHTLEG;
                    break;
                case GLFW_KEY_W:
                    dir = UP;
                    break;
                case GLFW_KEY_S:
                    dir = DOWN;
                    break;
                case GLFW_KEY_A:
                    dir = LEFT;
                    break;
                case GLFW_KEY_D:
                    dir = RIGHT;
                    break;
                case GLFW_KEY_Q:
                    dir = FORWARD;
                    break;
                case GLFW_KEY_E:
                    dir = BACKWARD;
                    break;
            }
        } else if (GLFW_RELEASE == action) {
            if (key == GLFW_KEY_W ||
                key == GLFW_KEY_S ||
                key == GLFW_KEY_A ||
                key == GLFW_KEY_D ||
                key == GLFW_KEY_Q ||
                key == GLFW_KEY_E) {
                stop();
            }
        }
    }
 public:
    bool start_flag = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_update;
    BodyPart selected = CAMERA;
    MoveDir dir = UP;
    float speed = 40.f;
    float camera_speed = 1.f;
};


// class BlobbyManAnimate {
//  public:
//     getStateByTime(float t);
//  public:
//     BlobbyMan start;
//     BlobbyMan end;
//     float total_time;
// };


class Drawable {
 public:
    virtual void draw(Program& program, Matrix matrix) = 0;
};

class Line : public Drawable {
 public:
    Line();
    ~Line();
    void draw(Program& program, Matrix matrix) override;
 public:
    std::vector<glm::vec3> vertices;
    VertexBufferObject vbo;
};

class Square : public Drawable {
 public:
    Square();
    ~Square();
    void draw(Program& program, Matrix matrix) override;
 public:
    std::vector<glm::vec3> vertices;
    VertexBufferObject vbo;
};

class GPlane : public Drawable {
 public:
    GPlane();
    void draw(Program& program, Matrix matrix) override;
 public:
    Square square;
    Line line;
};


class Sphere : public Drawable {
 public:
    Sphere(float r = 1.f);
    ~Sphere();
    void draw(Program& program, Matrix matrix) override;
 public:
    std::vector<glm::vec3> vertices;
    VertexBufferObject vbo;
    float radius;
};

class Head : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class UpArm : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class LowArm : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class Hand : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class Calf : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class Foot : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};

class Thigh : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
};




class LeftArm : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    UpArm up_arm;
    LowArm low_arm;
    Hand hand;
};

class RightArm : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    UpArm up_arm;
    LowArm low_arm;
    Hand hand;
};

class LeftLeg : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Thigh thigh;
    Calf calf;
    Foot foot;
};

class RightLeg : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Thigh thigh;
    Calf calf;
    Foot foot;
};

class Shoulder : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
    Head head;  
    LeftArm left_arm;
    RightArm right_arm;
};

class Body : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Sphere sphere;
    Shoulder shoulder;
};

class Torso : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    LeftLeg left_leg;
    RightLeg right_leg;
    Sphere sphere;
    Body body;
};

class Scene : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    GPlane gplane;
    Torso torso;
};

class World : public Drawable {
 public:
    void draw(Program& program, Matrix matrix) override;
 public:
    Scene scene;
};

}


#endif  // __BLOBBY_MAN_H__
