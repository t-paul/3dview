#ifndef TRANSFORM_H
#define	TRANSFORM_H

#include <glm/glm.hpp>

class Transform {
public:
    Transform();
    virtual ~Transform();

    void setCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
    void setWindow(float width, float height);
    void setTranslate(glm::vec3 translate);
    void setRotate(glm::vec3 rotate);
    void setScale(glm::vec3 scale);
    void setUniformScale(float scale);
    
    glm::vec3 up();
    glm::vec3 pos();
    glm::vec3 target();
    glm::mat4 matrix();
    glm::mat4 projection();
    
private:
    float width;
    float height;
    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;
    glm::vec3 cam_pos;
    glm::vec3 cam_target;
    glm::vec3 cam_up;
};

#endif	/* TRANSFORM_H */

