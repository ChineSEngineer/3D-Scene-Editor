#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "Helpers.h"

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp>

namespace CSGY6533 {

class Object {
 public:
    Object();
    void free();
    void draw(Program& program);
    void loadFromOffFile(const std::string& path);
    void unitize();
    void update();

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z);
    void scale(float change);
    void color(glm::vec3& color);

    bool intersectRay(const glm::vec3& e, const glm::vec3& d) const;

    glm::mat4 getModelMatrix() const;

 private:
    static bool intersectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
                                  const glm::vec3& e, const glm::vec3& d);
 private:
    std::vector<glm::vec3> m_vertices;
    std::vector<int> m_indices;
    std::vector<float> m_model; // 0,1,2 - translate, 3,4,5 - rotate, 6 - sacle
    glm::vec3 m_color; // 0,1,2 - rgb
    VertexBufferObject m_vbo;
    ElementBufferObject m_ebo;
};

class Geometry {
 public:
    Geometry();
    void init();
    void free();
    void bind();
    void draw(Program& program);
    void addObjFromOffFile(const std::string& path);

    void addBunny();
    void addBumpyCube();
    void addCube();
    void deleteObject(int index);

    size_t size() const;
    const Object& operator[](size_t index) const;
    Object& operator[](size_t index);
 private:
    std::vector<Object> m_objs;
    VertexArrayObject m_vao;
};

} // CSGY6533

#endif  // __GEOMETRY_H__
