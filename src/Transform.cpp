#include <stdio.h>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

Transform::Transform()
{
    width = 100;
    height = 100;
    translate = glm::vec3(0, 0, 0);
    rotate = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
}

Transform::~Transform()
{
}

void
Transform::setCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    this->cam_pos = pos;
    this->cam_target = target;
    this->cam_up = up;
}

void
Transform::setWindow(float width, float height)
{
    this->width = width;
    this->height = height;
}

void
Transform::setTranslate(glm::vec3 translate)
{
    this->translate = translate;
}

void
Transform::setRotate(glm::vec3 rotate)
{
    this->rotate = rotate;
}

void
Transform::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

void
Transform::setUniformScale(float scale)
{
    setScale(glm::vec3(scale, scale, scale));
}

glm::mat4
Transform::matrix()
{
    glm::mat4 p = glm::perspectiveFov(.52f, width, height, 1.0f, 100000.0f);
    glm::mat4 c = glm::lookAt(cam_pos, cam_target, cam_up);
    
    glm::mat4 t = glm::translate(glm::mat4(), translate);
    glm::mat4 s = glm::scale(t, scale);
    glm::mat4 rx = glm::rotate(s, rotate.x, glm::vec3(1, 0, 0));
    glm::mat4 ry = glm::rotate(rx, rotate.y, glm::vec3(0, 1, 0));
    glm::mat4 rz = glm::rotate(ry, rotate.z, glm::vec3(0, 0, 1));
    return p * c * rz;
}

glm::vec3
Transform::up()
{
    return cam_up;
}

glm::vec3
Transform::pos()
{
    return cam_pos;
}

glm::vec3
Transform::target()
{
    return cam_target;
}