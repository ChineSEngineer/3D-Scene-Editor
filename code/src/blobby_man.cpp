#include "blobby_man.h"

#include "geometry.h"
#include "macro.h"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>



namespace CSGY6533 {

static float FOV    = 45.f  ; static float ZN     = 0.17f ;
static float ZF     = 10.7f ;

static float XSCR   = -0.1f ;
static float YSCR   = -1.6f ;
static float ZSCR   = 7.9f  ;

static float BACK   = -90   ;
static float SPIN   = -30   ;
static float TILT   = 0     ;

static float XLOOK  = 0.f   ;
static float YLOOK  = 0.f   ;
static float ZLOOK  = 0.f   ;

static float XM     = 0.f   ;
static float YM     = 0.f   ;
static float ZM     = 1.75f ;
static float RZM    = 0.f   ;



static float EXTEN  = 0.f;
static float BTWIS  = 0.f;
static float ROT    = 0.f;

static float LELBO  = 0.f;
static float LATWIS = 0.f;
static float LSHOU  = 0.f;
static float LSID   = 0.f;
static float RELBO  = 0.f;
static float RATWIS = 0.f;
static float RSHOU  = 0.f;
static float RSID   = 0.f;

static float LHIP   = 0.f;
static float LOUT   = 0.f;
static float LTWIS  = 0.f;
static float LKNEE  = 0.f;
static float LANKL  = 0.f;
static float RHIP   = 0.f;
static float ROUT   = 0.f;
static float RTWIS  = 0.f;
static float RKNEE  = 0.f;
static float RANKL  = 0.f;

static float NOD    = 0.f;
static float NECK   = 0.f;

static float SWIDTH = 0.f;
static float SHEIGHT= 0.f;

static glm::vec3 DEFAULT_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 HEAD_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 BODY_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 LEFT_LEG_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 RIGHT_LEG_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 LEFT_ARM_COLOR = {0.f, 0.f, 0.f};
static glm::vec3 RIGHT_ARM_COLOR = {0.f, 0.f, 0.f};

void BlobbyMan::resize_window(int width, int height) {
    SWIDTH  = width;
    SHEIGHT = height;
}

void BlobbyMan::camera_move(MoveDir dir, float angle) {
    if (dir == UP) {
        YSCR -= angle;
    } else if (dir == DOWN) {
        YSCR += angle;
    } else if (dir == LEFT) {
        XSCR += angle;
    } else if (dir == RIGHT) {
        XSCR -= angle;
    } else if (dir == FORWARD) {
        ZSCR -= angle;
    } else if (dir == BACKWARD) {
        ZSCR += angle;
    }
}

void BlobbyMan::head_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add(NOD, angle);
    } else if (dir == DOWN) {
        degree_add(NOD, -angle);
    } else if (dir == LEFT) {
        degree_add(NECK, angle);
    } else if (dir == RIGHT) {
        degree_add(NECK, -angle);
    }
}

void BlobbyMan::left_leg_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add3(LOUT, -angle);
    } else if (dir == DOWN) {
        degree_add3(LOUT, angle);
    } else if (dir == LEFT) {
        degree_add(LHIP, angle);
    } else if (dir == RIGHT) {
        degree_add(LHIP, -angle);
    } else if (dir == FORWARD) {
        degree_add6(LKNEE, angle);
    } else if (dir == BACKWARD) {
        degree_add6(LKNEE, -angle);
    }

}

void BlobbyMan::right_leg_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add2(ROUT, angle);
    } else if (dir == DOWN) {
        degree_add2(ROUT, -angle);
    } else if (dir == LEFT) {
        degree_add(RHIP, -angle);
    } else if (dir == RIGHT) {
        degree_add(RHIP, angle);
    } else if (dir == FORWARD) {
        degree_add6(RKNEE, angle);
    } else if (dir == BACKWARD) {
        degree_add6(RKNEE, -angle);
    }

}

void BlobbyMan::left_arm_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add4(LSHOU, angle);
    } else if (dir == DOWN) {
        degree_add4(LSHOU, -angle);
    } else if (dir == LEFT) {
        degree_add4(LSID, -angle);
    } else if (dir == RIGHT) {
        degree_add4(LSID, angle);
    } else if (dir == FORWARD) {
        degree_add5(LELBO, angle);
    } else if (dir == BACKWARD) {
        degree_add5(LELBO, -angle);
    }
}

void BlobbyMan::right_arm_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add4(RSHOU, angle);
    } else if (dir == DOWN) {
        degree_add4(RSHOU, -angle);
    } else if (dir == LEFT) {
        degree_add4(RSID, -angle);
    } else if (dir == RIGHT) {
        degree_add4(RSID, angle);
    } else if (dir == FORWARD) {
        degree_add5(RELBO, angle);
    } else if (dir == BACKWARD) {
        degree_add5(RELBO, -angle);
    }

}

void BlobbyMan::body_move(MoveDir dir, float angle) {
    if (dir == UP) {
        degree_add(EXTEN, -angle);
    } else if (dir == DOWN) {
        degree_add(EXTEN, angle);
    } else if (dir == LEFT) {
        degree_add(BTWIS, angle);
    } else if (dir == RIGHT) {
        degree_add(BTWIS, -angle);
    } else if (dir == FORWARD) {
        degree_add7(ROT, angle);
    } else if (dir == BACKWARD) {
        degree_add7(ROT, -angle);
    }

}

void BlobbyMan::update_color() {
    HEAD_COLOR = BODY_COLOR = LEFT_ARM_COLOR = 
    RIGHT_ARM_COLOR = LEFT_LEG_COLOR = RIGHT_LEG_COLOR =
    DEFAULT_COLOR;
    switch (selected) {
        case HEAD:
            HEAD_COLOR = {1.f, 0.f, 0.f};
            break;
        case BODY:
            BODY_COLOR = {1.f, 0.f, 0.f};
            break;
        case LEFTARM:
            LEFT_ARM_COLOR = {1.f, 0.f, 0.f};
            break;
        case RIGHTARM:
            RIGHT_ARM_COLOR = {1.f, 0.f, 0.f};
            break;
        case LEFTLEG:
            LEFT_LEG_COLOR = {1.f, 0.f, 0.f};
            break;
        case RIGHTLEG:
            RIGHT_LEG_COLOR = {1.f, 0.f, 0.f};
            break;
    }
}

void BlobbyMan::update() {
    update_color();
    if (!start_flag) return;
    auto t_now = std::chrono::high_resolution_clock::now(); 
    float elapse = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - last_update).count();
    last_update = t_now;
    float degree = elapse * speed;
    float distance = elapse * camera_speed;

    switch (selected) {
        case CAMERA:
            camera_move(dir, distance);
            break;
        case HEAD:
            head_move(dir, degree);
            break;
        case BODY:
            body_move(dir, degree);
            break;
        case LEFTARM:
            left_arm_move(dir, degree);
            break;
        case RIGHTARM:
            right_arm_move(dir, degree);
            break;
        case LEFTLEG:
            left_leg_move(dir, degree);
            break;
        case RIGHTLEG:
            right_leg_move(dir, degree);
            break;
    }
}

glm::mat4 get_aspect_ratio_matrix() {
    return glm::scale(glm::mat4(1.f), glm::vec3(static_cast<float>(SHEIGHT) / static_cast<float>(SWIDTH), 1.f, 1.f));
}

     //   float EXTEN = -90.f ;
     //   float BTWIS  = 0.f   ;
     //   float ROT    = 0.f   ;
     //   float LELBO  = 90.f  ;
     //   float RELBO  = 85.f  ;
     //   float LATWIS = -90.f ;
     //   float RATWIS = -102.f;
     //   float LSHOU  = 0.f   ;
     //   float RSHOU  = 40.f  ;
     //   float NOD    = -25.f ;
     //   float NECK   = 28.f  ;
     //   float LSID   = -45.f ;
     //   float RSID   = 112.f ;
     //   float LHIP   = 0.f   ;
     //   float RHIP   = 105.f ;
     //   float LOUT   = 0.f   ;
     //   float ROUT   = 13.f  ;
     //   float LTWIS  = 0.f   ;
     //   float RTWIS  = -86.f ;
     //   float LKNEE  = 0.f   ;
     //   float RKNEE  = -53.f ;
     //   float LANKL  = 0.f   ;
     //   float RANKL  = 0.f   ;

//static int SELECTED = -1;

Line::Line() {
    vbo.init();
    vbo.bind();
    vertices.emplace_back(0.f, 0.f, 0.f);
    vertices.emplace_back(1000.f, 0.f, 0.f);
    vbo.update(vertices);
}


Line::~Line() { vbo.free(); }

void Line::draw(Program& program, Matrix matrix) {
    program.bind();
    GLint uniMVP = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(matrix.matrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(get_aspect_ratio_matrix()));
    program.bindVertexAttribArray("position",this->vbo);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

Square::Square() {
    vbo.init();
    vbo.bind();
    vertices.emplace_back(0.f, 0.f, 0.f);
    vertices.emplace_back(0.f, 1.f, 0.f);
    vertices.emplace_back(1.f, 1.f, 0.f);
    vertices.emplace_back(1.f, 0.f, 0.f);
    vertices.emplace_back(0.f, 0.f, 0.f);
    vbo.update(vertices);
}


Square::~Square() { vbo.free(); }

void Square::draw(Program& program, Matrix matrix) {
    program.bind();
    GLint uniMVP = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(matrix.matrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(get_aspect_ratio_matrix()));
    program.bindVertexAttribArray("position",this->vbo);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}

GPlane::GPlane() { }

void GPlane::draw(Program& program, Matrix matrix) {
    for (int x = -2; x < 2; x += 1) {
        for (int y = -2; y < 2; y += 1) {
            square.draw(program, matrix.tran((float)x, (float)y, 0.f));
        }
    }
    line.draw(program, matrix);
    line.draw(program, matrix.rot(90.f, 2).tran(0.f, 2.f, 0.f));
    line.draw(program, matrix.rot(-90.f, 3));
}

Sphere::Sphere(float r) : radius{r} {
    vbo.init();
    vbo.bind();
    float step = 0.08 / r, pi = (float) M_PI;
    for (float angle = 0; angle < 2 * pi; angle += step) {
        vertices.emplace_back(radius * cos(angle), radius * sin(angle), 0.f);
    }
    vertices.emplace_back(radius, 0.f, 0.f);
    for (float angle = 0; angle < 2 * pi; angle += step) {
        vertices.emplace_back(0.f, radius * cos(angle), radius * sin(angle));
    }
    vertices.emplace_back(0.f, radius, 0.f);
    for (float angle = 0; angle < 2 * pi; angle += step) {
        vertices.emplace_back(radius * cos(angle), 0.f, radius * sin(angle));
    }
    vertices.emplace_back(radius, 0.f, 0.f);
    vbo.update(vertices);
}

Sphere::~Sphere() { vbo.free(); }

void Sphere::draw(Program& program, Matrix matrix) {
    program.bind();
    GLint uniMVP = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(matrix.matrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(get_aspect_ratio_matrix()));
    program.bindVertexAttribArray("position",vbo);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);
    glDrawArrays(GL_LINE_STRIP, vertices.size() / 3, vertices.size() / 3);
    glDrawArrays(GL_LINE_STRIP, vertices.size() * 2 / 3, vertices.size() / 3);
}

void Head::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.tran(0.f,0.f,0.4f)
                           .scal(0.2, 0.23, 0.3));
    sphere.draw(program, matrix.tran(0.f, -0.255, 0.42)
                           .scal(0.035f, 0.075f, 0.035f));
    sphere.draw(program, matrix.tran(0.f, 0.f, 0.07f)
                           .scal(0.065f, 0.065f, 0.14f));
    sphere.draw(program, matrix.tran(0.f, -0.162f, 0.239f)
                           .scal(0.0533f, 0.0508f, 0.0506f));
}

void UpArm::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.tran(0.f, 0.f, -0.275f)
                           .scal(0.09f, 0.09f, 0.275f));
}

void LowArm::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.tran(0.f, 0.f, -0.25f)
                           .scal(0.08f, 0.08f, 0.25f));
}

void Hand::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.tran(0.f, 0.f, -0.116f)
                          .scal(0.052f, 0.091f, 0.155f));
}

void Thigh::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.tran(0.f, 0.f, -0.425f)
                          .scal(0.141f, 0.141f, 0.425f));
}

void Calf::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.scal(0.05f, 0.05f, 0.05f));
    sphere.draw(program, matrix.tran(0.f, 0.f, -0.425f)
                           .scal(0.1f, 0.1f, 0.425f));
}

void Foot::draw(Program& program, Matrix matrix) {
    sphere.draw(program, matrix.scal(0.05f, 0.04f, 0.04f));
    sphere.draw(program, matrix.tran(0.f, 0.05f, -0.05f)
                           .scal(0.04f, 0.04f, 0.04f));
    sphere.draw(program, matrix.tran(0.f, -0.15f, -0.05f)
                           .rot(-10.f, 1)
                           .scal(0.08f, 0.19f, 0.05f));
}

void LeftArm::draw(Program& program, Matrix matrix) {
    up_arm.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.55f).rot(LELBO, 1);
    low_arm.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.5f);
    hand.draw(program, matrix);
}

void RightArm::draw(Program& program, Matrix matrix) {
    up_arm.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.55f).rot(RELBO, 1);
    low_arm.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.5f);
    hand.draw(program, matrix);
}

void LeftLeg::draw(Program& program, Matrix matrix) {
    matrix = matrix.rot(LHIP, 3).rot(LOUT,2).rot(-LHIP, 3).rot(LTWIS, 3);
    thigh.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.85f).rot(LKNEE, 1);
    calf.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.84f).rot(LANKL, 1);
    foot.draw(program, matrix);
}

void RightLeg::draw(Program& program, Matrix matrix) {
    matrix = matrix.rot(RHIP, 3).rot(ROUT, 2).rot(-RHIP, 3).rot(RTWIS, 3);
    thigh.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.85f).rot(RKNEE, 1);
    calf.draw(program, matrix);
    matrix = matrix.tran(0.f, 0.f, -0.84f).rot(RANKL, 1);
    foot.draw(program, matrix);
}

void Shoulder::draw(Program& program, Matrix matrix) {
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(HEAD_COLOR));
    sphere.draw(program, matrix.scal(0.45f, 0.153f, 0.12f));
    head.draw(program, matrix.tran(0.f, 0.f, 0.153f)
                             .rot(NOD, 1).rot(NECK,3));
    glUniform3fv(uniColor, 1, glm::value_ptr(LEFT_ARM_COLOR));
    left_arm.draw(program, matrix.tran(-0.45f, 0.f, 0.f)
                                 .rot(LSID, 2).rot(LSHOU, 1)
                                 .rot(LATWIS, 3));
    glUniform3fv(uniColor, 1, glm::value_ptr(RIGHT_ARM_COLOR));
    right_arm.draw(program, matrix.tran(0.45f, 0.f, 0.f)
                                   .rot(RSID, 2).rot(RSHOU, 1)
                                   .rot(RATWIS, 3));
}

void Body::draw(Program& program, Matrix matrix) {
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(BODY_COLOR));
    sphere.draw(program, matrix.tran(0.f, 0.f, 0.62f)
                           .scal(0.306f, 0.21f, 0.5f));
    glUniform3fv(uniColor, 1, glm::value_ptr(DEFAULT_COLOR));
    shoulder.draw(program, matrix.tran(0.f, 0.f, 1.f)
                                 .rot(EXTEN,1).rot(BTWIS, 2)
                                 .rot(ROT,3));
}

void Torso::draw(Program& program, Matrix matrix) {
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(LEFT_LEG_COLOR));
    left_leg.draw(program, matrix.tran(-0.178f, 0.f, 0.f));
    glUniform3fv(uniColor, 1, glm::value_ptr(RIGHT_LEG_COLOR));
    right_leg.draw(program, matrix.tran(0.178f, 0.f, 0.f));
    glUniform3fv(uniColor, 1, glm::value_ptr(BODY_COLOR));
    sphere.draw(program, matrix.tran(0.f, 0.f, 0.08f).scal(0.275f, 0.152f, 0.153f));
    glUniform3fv(uniColor, 1, glm::value_ptr(DEFAULT_COLOR));
    body.draw(program, matrix.rot(EXTEN, 1).rot(BTWIS, 2).rot(ROT, 3));
}

void Scene::draw(Program& program, Matrix matrix) {
    program.bind();
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(DEFAULT_COLOR));
    gplane.draw(program, matrix);
    torso.draw(program, matrix.tran(XM, YM,ZM)
                              .rot(RZM, 3));
}

void World::draw(Program& program, Matrix matrix) {
    matrix = matrix
                   .pers(FOV, ZN, ZF)
                   .tran(XSCR, YSCR, -ZSCR)
                   .rot(BACK, 1).rot(SPIN, 3).rot(TILT, 1)
                   .tran(-XLOOK, -YLOOK, -ZLOOK).scal(1.f, 1.f, -1.f);
    scene.draw(program, matrix);
}

}
