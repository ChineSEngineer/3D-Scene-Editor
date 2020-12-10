#include "skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/gtc/type_ptr.hpp> 
namespace CSGY6533 {

void Skybox::init() {
    m_vao.init();
    m_vbo.init();
    m_texture.init();
}

void Skybox::free() {
    m_vao.free();
    m_vbo.free();
    m_texture.free();
}

void Skybox::bind() {
    m_vao.bind();
}

void Skybox::update() {
    m_vbo.update(m_vertices);
}

void Skybox::configCubeMap() {
    m_texture.bind(GL_TEXTURE_CUBE_MAP);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < m_images.size(); i++)
    {
        unsigned char *data = stbi_load(m_images[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << m_images[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(Program& program, ViewControl& view_control) {
    program.bind();
    GLint uniProjection = program.uniform("projection");
    glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(view_control.getProjMatrix()));
    GLint uniView = program.uniform("view");
    glm::mat4 view = glm::mat4(glm::mat3(view_control.getViewMatrix()));
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    GLint uniAR = program.uniform("AspectRatioMatrix");
    glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(view_control.getAspectRatioMatrix()));
    glActiveTexture(GL_TEXTURE0);
    m_texture.bind(GL_TEXTURE_CUBE_MAP);

    program.bindVertexAttribArray("position",m_vbo);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

Texture Skybox::getTexture() const {
    return m_texture;
}

std::vector<glm::vec3> Skybox::m_vertices = {
    {-1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},

    {-1.0f, -1.0f,  1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},

    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},

    {-1.0f, -1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},

    {-1.0f,  1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f, -1.0f},

    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f}
};

std::vector<std::string> Skybox::m_images = {
    "../../data/day_posx.jpg",
    "../../data/day_negx.jpg",
    "../../data/day_posy.jpg",
    "../../data/day_negy.jpg",
    "../../data/day_posz.jpg",
    "../../data/day_negz.jpg"
};

}  // CSGY6533
