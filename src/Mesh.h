#ifndef MESH_H
#define	MESH_H

#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(const std::string fileName);
    virtual ~Mesh();
    
    void bind();
    void draw();
    
    const glm::vec3 & bboxMin();
    const glm::vec3 & bboxMax();
    
private:
    std::string fileName;
    unsigned int count;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint normalBuffer;
    GLuint bcoordsBuffer;
    glm::vec3 min, max;
};

#endif	/* MESH_H */

