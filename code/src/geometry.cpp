#include "geometry.h"

#include "off_reader.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <algorithm>

namespace CSGY6533 {

Object::Object() : m_model {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f}
                 , m_color {0.f, 0.f, 0.f} {
    m_vbo.init();
    m_ebo.init();
}

void Object::free() {
    m_vbo.free();
    m_ebo.free();
}

void Object::draw(Program& program) {
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(m_color));
    GLint uniMVP = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
    program.bindVertexAttribArray("position",m_vbo);
    m_ebo.bind();
    glUniform3f(program.uniform("triangleColor"), 0.0f, 0.0f, 0.0f);
    glDrawElements(GL_TRIANGLES, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    // glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
    // glDrawElements(GL_LINE_LOOP, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Object::loadFromOffFile(const std::string& path) {
    auto p = OffReader::read(path);
    m_vertices = p.first;
    m_indices = p.second;
    for (int i = 0; i < m_indices.size(); i+=3) {
        std::cout << m_indices[i] << " " << m_indices[i + 1] << " " << m_indices[i + 2] << std::endl;
    }
    for (int i = 0; i < m_vertices.size(); i+=1) {
        // m_vertices[i] *= 0.2;
        // m_vertices[i][1] -= 0.5;
        std::cout << glm::to_string(m_vertices[i]) << std::endl;
    }
}

void Object::update() {
    m_vbo.update(m_vertices);
    m_ebo.update(m_indices);
}

void Object::translate(float x, float y, float z) {
    m_model[0] += x;
    m_model[1] += y;
    m_model[2] += z;
}

void Object::rotate(float x, float y, float z) {
    m_model[3] += x;
    m_model[4] += y;
    m_model[5] += z;
}

void Object::scale(float change) { m_model[6] += change; }

void Object::color(glm::vec3& color) {
    m_color = {color[0], color[1], color[2]};
}

void Object::unitize() {
    float min_x, min_y, min_z, max_x, max_y, max_z;
    min_x = min_y = min_z = std::numeric_limits<float>::max();
    max_x = max_y = max_z = std::numeric_limits<float>::min();
    for (int i = 0; i < m_vertices.size(); ++i) {
        min_x = std::min(min_x, m_vertices[i][0]);
        min_y = std::min(min_y, m_vertices[i][1]);
        min_z = std::min(min_z, m_vertices[i][2]);

        max_x = std::max(max_x, m_vertices[i][0]);
        max_y = std::max(max_y, m_vertices[i][1]);
        max_z = std::max(max_z, m_vertices[i][2]);
    }
    std::cout << min_y << " " << max_y << std::endl;
    float scale = std::max({max_x - min_x, max_y - min_y, max_z - min_z});
    for (int i = 0; i < m_vertices.size(); ++i) {
        m_vertices[i][0] -= (min_x + max_x) / 2;
        m_vertices[i][1] -= (min_y + max_y) / 2;
        m_vertices[i][2] -= (min_z + max_z) / 2;
        if (scale != 0.f) {
            m_vertices[i][0] /= scale;
            m_vertices[i][1] /= scale;
            m_vertices[i][2] /= scale;
        }
    }
}

bool Object::intersectRay(const glm::vec3& e, const glm::vec3& d) const {
    glm::mat4 transform = getModelMatrix();
    for (int i = 0; i < m_indices.size(); i += 3) {
        glm::vec3 a = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i]], 1.f));
        glm::vec3 b = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i + 1]], 1.f));
        glm::vec3 c = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i + 2]], 1.f));
        // a,b,c,e,d are al world coordinate
        if (intersectTriangle(a, b, c, e, d)) {
            return true;
        }
    }
    return false;
}

glm::mat4 Object::getModelMatrix() const {
    float translate_x = m_model[0];
    float translate_y = m_model[1];
    float translate_z = m_model[2];
    float rotate_x = m_model[3];
    float rotate_y = m_model[4];
    float rotate_z = m_model[5];
    float scale = m_model[6];
    glm::vec3 barycentre = {0.f, 0.f, 0.f};
    glm::mat4 res = glm::translate(glm::mat4(1.f),  glm::vec3(translate_x, translate_y, translate_z)) *
                    glm::translate(glm::mat4(1.f), barycentre) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_x), glm::vec3(1.f,0.f,0.f)) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_y), glm::vec3(0.f,1.f,0.f)) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_z), glm::vec3(0.f,0.f,1.f)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(scale)) *
                    glm::translate(glm::mat4(1.f), - barycentre);
    return res;
}


bool Object::intersectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
                               const glm::vec3& e, const glm::vec3& d) {
    // Textbook 4.4.2
    float near = 0.f, far = 2.f; // TODO: pass in
    float da = glm::determinant(glm::mat3(a - b, a - c, d));
    float t = glm::determinant(glm::mat3(a - b, a - c, a - e)) / da; //t
    if (t < near || t > far) { return false; }
    float gamma = glm::determinant(glm::mat3(a - b, a - e, d)) / da; // gamma
    if (gamma < 0 || gamma > 1) { return false; }
    float beta = glm::determinant(glm::mat3(a - e, a - c, d)) / da; //beta
    if (beta < 0 || beta > 1 - gamma) { return false; }
    return true;
}


Geometry::Geometry() {}

void Geometry::init() {
    m_vao.init();
}

void Geometry::free() {
    m_vao.free();
    for (auto&& obj : m_objs) {
        obj.free();
    }
}

void Geometry::bind() {
    m_vao.bind();
}

void Geometry::draw(Program& program) {
    for (auto&& obj : m_objs) {
        obj.draw(program);
    }
}

void Geometry::addObjFromOffFile(const std::string& path) {
    Object obj; 
    obj.loadFromOffFile(path);
    obj.unitize();
    obj.update();
    m_objs.push_back(obj);
}

void Geometry::addBunny() {
    addObjFromOffFile("../../data/bunny.off");
}

void Geometry::addBumpyCube() {
    addObjFromOffFile("../../data/bumpy_cube.off");
}

void Geometry::addCube() {
    addObjFromOffFile("../../data/cube.off");
}

void Geometry::deleteObject(int index) {
    ASSERT(index < m_objs.size(), "deleteObject(index): index out of range");
    m_objs.erase(m_objs.begin() + index);
}

size_t Geometry::size() const { return m_objs.size(); }

const Object& Geometry::operator[](size_t index) const {
    return m_objs[index];
}

Object& Geometry::operator[](size_t index) {
   return m_objs[index];
}

}  // CSGY6533
