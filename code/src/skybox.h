#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Helpers.h"
#include "geometry.h"
#include "view_control.h"


namespace CSGY6533 {

class Skybox {
 public:
    void init();
    void free();
    void bind();
    void update();
    void configCubeMap();
    void draw(Program& program, ViewControl& view_control);
    Texture getTexture() const;
 private:
    VertexArrayObject m_vao;
    VertexBufferObject m_vbo;
    Texture m_texture;
    static std::vector<glm::vec3> m_vertices;
    static std::vector<std::string> m_images;
};



}  // CSGY6533

#endif  // __SKYBOX_H__
