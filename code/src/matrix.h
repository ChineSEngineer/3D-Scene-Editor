#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "macro.h"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace CSGY6533 {

class Matrix {
 public:
    Matrix() : matrix{1.f} {}
    Matrix(glm::mat4& other) : matrix{other} {}

    Matrix tran(float x, float y, float z) const {
        glm::mat4 res = glm::translate(matrix, glm::vec3(x, y, z));
        return res;
    }

    Matrix scal(float sx, float sy, float sz) {
        glm::mat4 res = glm::scale(matrix, glm::vec3(sx, sy, sz));
        return res;
    }
    
    Matrix rot(float theta, int j) {
        glm::mat4 res;
        if (j == 1) {
            res = glm::rotate(matrix, glm::radians(-theta), glm::vec3(1.f, 0.f, 0.f));
        } else if (j == 2) {
            res = glm::rotate(matrix, glm::radians(-theta), glm::vec3(0.f, 1.f, 0.f));
        } else if (j == 3) {
            res = glm::rotate(matrix, glm::radians(-theta), glm::vec3(0.f, 0.f, 1.f));
        } else {
            ASSERT(false, "rot: j must be 1 ~ 3");
        }
        return res;
    }

    Matrix pers(float fov, float near, float far) {
        glm::mat4 res = glm::perspective(fov, 1.f, near, far) * matrix;
        return res;
    }

    Matrix orie(float a, float b, float c,
                          float d, float e, float f,
                          float p, float q, float r) {
        glm::mat4 res = glm::mat4({a,b,c,0.f}, {d,e,f,0.f},
                            {p,q,r,0.f}, {0.f,0.f,0.f,1.f}) * matrix;
        return res;
    }
 public:
    glm::mat4 matrix;
};

}  // CSGY6533

#endif  // __MATRIX_H__
