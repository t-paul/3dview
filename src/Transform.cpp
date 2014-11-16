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
    glm::mat4 view;
    //view = glm::translate(view, glm::vec3(0.0, 0.0, 0.0));
    view = glm::translate(view, translate);
    view = glm::rotate(view, rotate.z, glm::vec3(0.0, 0.0, 1.0));
    view = glm::rotate(view, rotate.y, glm::vec3(0.0, 1.0, 0.0));
    view = glm::rotate(view, rotate.x, glm::vec3(1.0, 0.0, 0.0));
    
    return view;
}

glm::mat4
Transform::projection()
{
    float fov = M_PI * 75.0 / 180.0;
    glm::mat4 projection = glm::perspectiveFov(fov, width, height, 1.0f, 10000.0f);
    return projection;
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