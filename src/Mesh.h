#pragma once

#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(const std::string fileName, bool generateVertexNormals = false);
    virtual ~Mesh();
    
    void bind();
    glm::mat4 matrix() const;
    void draw(GLuint vertex, GLuint normal, GLuint texture) const;
    
    const glm::vec3 & bboxMin();
    const glm::vec3 & bboxMax();
    
private:
    std::string fileName;
    bool generateVertexNormals;
    unsigned int count;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint textureBuffer;
    glm::vec3 min, max;
};
