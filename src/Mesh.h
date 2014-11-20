#pragma once

#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(const std::string fileName);
    virtual ~Mesh();
    
    void bind();
    glm::mat4 matrix() const;
    void draw(GLuint vertex, GLuint normal) const;
    
    const glm::vec3 & bboxMin();
    const glm::vec3 & bboxMax();
    
private:
    std::string fileName;
    unsigned int count;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    glm::vec3 min, max;
};
