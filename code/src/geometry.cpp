#include "geometry.h"

#include "off_reader.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <limits>
#include <algorithm>

namespace CSGY6533 {

Object::Object() : m_model {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f}
                 , m_color {0.2f, 0.2f, 0.2f}
                 , m_mode {MODE1} {
    m_vbo.init();
    m_ebo.init();
    m_nbo.init();
}

void Object::free() {
    m_vbo.free();
    m_ebo.free();
    m_nbo.free();
}

void Object::draw(std::vector<Program>& programs, glm::vec3& light, ViewControl& view_control, Texture& depth_texture) {
    // getShadowMap(programs[SHADOW], light, view_control, depth_fbo, depth_texture);
    if (m_mode == MODE1) {
        drawWireframe(programs[WIREFRAME], light, view_control);
    } else if (m_mode == MODE2) {
        drawFlatShading(programs[FLAT], light, view_control, depth_texture);
        drawWireframe(programs[WIREFRAME], light, view_control);
    } else if (m_mode == MODE3) {
        drawPhongShading(programs[PHONG], light, view_control, depth_texture);
    }
}

void Object::simpleDraw(Program& program) {
    program.bind();
    GLint uniModelMatrix = program.uniform("ModelMatrix");
    glUniformMatrix4fv(uniModelMatrix, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

    program.bindVertexAttribArray("position",m_vbo);

    m_ebo.bind();
    glDrawElements(GL_TRIANGLES, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Object::drawWireframe(Program& program, glm::vec3& light, ViewControl& view_control) {
    program.bind();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glm::mat4 MVPMatrix = view_control.getProjMatrix() *
                          view_control.getViewMatrix() *
                          getModelMatrix();
    GLint uniColor = program.uniform("Color");
    glUniform3fv(uniColor, 1, glm::value_ptr(m_color));
    GLint uniMVP = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(view_control.getAspectRatioMatrix()));

    program.bindVertexAttribArray("position",m_vbo);

    m_ebo.bind();
    glDrawElements(GL_TRIANGLES, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void Object::drawFlatShading(Program& program, glm::vec3& light, ViewControl& view_control, Texture& depth_texture) {
    program.bind();

    glm::mat4 MVPMatrix = view_control.getProjMatrix() *
                          view_control.getViewMatrix() *
                          getModelMatrix();
    GLint uniColor = program.uniform("color");
    glUniform3fv(uniColor, 1, glm::value_ptr(m_color));
    GLint uniMVPMatrix = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(view_control.getAspectRatioMatrix()));
    GLint uniModelMatrix = program.uniform("ModelMatrix");
    glUniformMatrix4fv(uniModelMatrix, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
    GLint uniEyePosition = program.uniform("eyePosition");
    glUniform3fv(uniEyePosition, 1, glm::value_ptr(view_control.getEyePosition())); 

    GLint uniFarPlane = program.uniform("far_plane");
    glUniform1f(uniFarPlane, view_control.far()); 
    GLint uniLightPosition = program.uniform("lightPosition");
    glUniform3fv(uniLightPosition, 1, glm::value_ptr(light)); 
    glActiveTexture(GL_TEXTURE0);
    depth_texture.bind(GL_TEXTURE_CUBE_MAP);

    program.bindVertexAttribArray("position",m_vbo);

    m_ebo.bind();
    glDrawElements(GL_TRIANGLES, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Object::drawPhongShading(Program& program, glm::vec3& light, ViewControl& view_control, Texture& depth_texture) {
    program.bind();

    glm::mat4 MVPMatrix = view_control.getProjMatrix() *
                          view_control.getViewMatrix() *
                          getModelMatrix();
    GLint uniColor = program.uniform("color");
    glUniform3fv(uniColor, 1, glm::value_ptr(m_color));
    GLint uniMVPMatrix = program.uniform("MVPMatrix");
    glUniformMatrix4fv(uniMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(view_control.getAspectRatioMatrix()));
    GLint uniModelMatrix = program.uniform("ModelMatrix");
    glUniformMatrix4fv(uniModelMatrix, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
    GLint uniNormalMatrix = program.uniform("NormalMatrix");
    glUniformMatrix3fv(uniNormalMatrix, 1, GL_FALSE, glm::value_ptr(getNormalMatrix()));
    GLint uniEyePosition = program.uniform("eyePosition");
    glUniform3fv(uniEyePosition, 1, glm::value_ptr(view_control.getEyePosition())); 

    GLint uniFarPlane = program.uniform("far_plane");
    glUniform1f(uniFarPlane, view_control.far()); 
    GLint uniLightPosition = program.uniform("lightPosition");
    glUniform3fv(uniLightPosition, 1, glm::value_ptr(light)); 
    glActiveTexture(GL_TEXTURE0);
    depth_texture.bind(GL_TEXTURE_CUBE_MAP);

    program.bindVertexAttribArray("position",m_vbo);
    program.bindVertexAttribArray("vertex_normal", m_nbo);

    m_ebo.bind();
    glDrawElements(GL_TRIANGLES, m_ebo.cols, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Object::loadFromOffFile(const std::string& path) {
    auto p = OffReader::read(path);
    m_vertices = p.first;
    m_indices = p.second;

    int n = m_vertices.size();
    m_vertex_normals = std::vector<glm::vec3>(n, glm::vec3(0.f));
    std::vector<int> count(n, 0);
    for (int i = 0; i < m_indices.size(); i += 3) {
        int index_a = m_indices[i];
        int index_b = m_indices[i + 1];
        int index_c = m_indices[i + 2];

        glm::vec3 a = m_vertices[index_a];
        glm::vec3 b = m_vertices[index_b];
        glm::vec3 c = m_vertices[index_c];
        
        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - b));
        m_vertex_normals[index_a] += normal;
        m_vertex_normals[index_b] += normal;
        m_vertex_normals[index_c] += normal;
        count[index_a] += 1;
        count[index_b] += 1;
        count[index_c] += 1;
    }
    for (int i = 0; i < n; ++i) {
        if (count[i] != 0) {
            m_vertex_normals[i] /= count[i];
        }
    }
}

void Object::update() {
    m_vbo.update(m_vertices);
    m_ebo.update(m_indices);
    m_nbo.update(m_vertex_normals);
}

void Object::setDisplayMode(DisplayMode mode) { m_mode =  mode; }

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

void Object::inverseColor() {
    m_color = glm::vec3(1.f) - m_color;
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

std::pair<bool, float> Object::intersectRay(const glm::vec3& e, const glm::vec3& d, float near, float far) const {
    float min_t = std::numeric_limits<float>::max();
    bool intersect = false;
    glm::mat4 transform = getModelMatrix();
    for (int i = 0; i < m_indices.size(); i += 3) {
        glm::vec3 a = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i]], 1.f));
        glm::vec3 b = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i + 1]], 1.f));
        glm::vec3 c = glm::vec3(transform * glm::vec4(m_vertices[m_indices[i + 2]], 1.f));
        // a,b,c,e,d are al world coordinate
        auto p = intersectTriangle(a, b, c, e, d, near, far);
        if (p.first) {
            intersect = true;
            min_t = std::min(min_t, p.second);
        }
    }
    return {intersect, min_t};
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
                    glm::translate(glm::mat4(1.f), - barycentre) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_x), glm::vec3(1.f,0.f,0.f)) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_y), glm::vec3(0.f,1.f,0.f)) *
                    glm::rotate(glm::mat4(1.f), glm::radians(rotate_z), glm::vec3(0.f,0.f,1.f)) *
                    glm::scale(glm::mat4(1.f), glm::vec3(scale)) *
                    glm::translate(glm::mat4(1.f), barycentre);
    return res;
}

glm::mat3 Object::getNormalMatrix() const {
    return glm::mat3(glm::transpose(glm::inverse(getModelMatrix())));
    // return glm::mat3(1.0);
}

std::pair<bool, float> Object::intersectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
                               const glm::vec3& e, const glm::vec3& d, float near, float far) {
    // Textbook 4.4.2
    float da = glm::determinant(glm::mat3(a - b, a - c, d));
    float t = glm::determinant(glm::mat3(a - b, a - c, a - e)) / da; //t
    if (t < near || t > far) { return {false, 0.f}; }
    float gamma = glm::determinant(glm::mat3(a - b, a - e, d)) / da; // gamma
    if (gamma < 0 || gamma > 1) { return {false, 0.f}; }
    float beta = glm::determinant(glm::mat3(a - e, a - c, d)) / da; //beta
    if (beta < 0 || beta > 1 - gamma) { return {false, 0.f}; }
    return {true, t};
}


Geometry::Geometry() : m_light {1.f, 1.f, 1.f} {}

void Geometry::init() {
    m_vao.init();
    m_depth_fbo.init();
    m_depth_texture.init();
}

void Geometry::free() {
    m_vao.free();
    for (auto&& obj : m_objs) {
        obj.free();
    }
    m_depth_fbo.free();
    m_depth_texture.free();
}

void Geometry::bind() {
    m_vao.bind();
}

void Geometry::configShadowMap() {
    m_depth_texture.bind(GL_TEXTURE_CUBE_MAP);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    m_depth_fbo.attach_depth_texture(m_depth_texture);
}

void Geometry::getShadowTexture(Program& program, glm::vec3& light, ViewControl& view_control) {
    m_depth_fbo.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    program.bind();
    std::vector<glm::mat4> shadowMatrices = view_control.getShadowMatrices(light);
    for (int i = 0; i < 6; ++i) {
        GLint uniShadowMatrix_i = program.uniform("shadowMatrices[" + std::to_string(i) + "]");
        glUniformMatrix4fv(uniShadowMatrix_i, 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));
    }
    GLint uniFarPlane = program.uniform("far_plane");
    glUniform1f(uniFarPlane, view_control.far()); 
    GLint uniLightPosition = program.uniform("lightPosition");
    glUniform3fv(uniLightPosition, 1, glm::value_ptr(light)); 
    for (auto&& obj : m_objs) {
        obj.simpleDraw(program);
    }
    m_depth_fbo.unbind();
}

void Geometry::draw(std::vector<Program>& programs, ViewControl& view_control) {
    glViewport(0, 0, 1024, 1024);
    getShadowTexture(programs[SHADOW], m_light, view_control);
    glViewport(0, 0, view_control.screenWidth(), view_control.screenHeight());
    for (auto&& obj : m_objs) {
        obj.draw(programs, m_light, view_control, m_depth_texture);
    }
}

void Geometry::addObjFromOffFile(const std::string& path) {
    Object obj; 
    obj.loadFromOffFile(path);
    obj.unitize();
    obj.update();
    obj.setDisplayMode(Object::DisplayMode::MODE3);
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

void Geometry::addPlane() {
    Object obj; 
    obj.loadFromOffFile("../../data/plane.off");
    obj.update();
    obj.setDisplayMode(Object::DisplayMode::MODE3);
    m_objs.push_back(obj);
}

void Geometry::deleteObject(int index) {
    ASSERT(index < m_objs.size(), "deleteObject(index): index out of range");
    m_objs.erase(m_objs.begin() + index);
}

int Geometry::intersectRay(const glm::vec3& e, const glm::vec3& d, float near, float far) const {
    float min_t = std::numeric_limits<float>::max();
    int res = -1;
    for (int i = 0; i < m_objs.size(); ++i) {
        auto p = m_objs[i].intersectRay(e, d, near, far);
        if (p.first) {
            if (p.second < min_t) {
                min_t = p.second;
                res = i;
            }
        }
    }
    return res;
}

size_t Geometry::size() const { return m_objs.size(); }

const Object& Geometry::operator[](size_t index) const {
    return m_objs[index];
}

Object& Geometry::operator[](size_t index) {
   return m_objs[index];
}

}  // CSGY6533
