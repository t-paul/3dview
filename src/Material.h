#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

class Material
{
private:
    std::string _name;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _shininess;
    
public:
    Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    virtual ~Material();
    
    const std::string & name() const;
    const glm::vec3 & ambientColor() const;
    const glm::vec3 & diffuseColor() const;
    const glm::vec3 & specularColor() const;
    const float shininess() const;
};

class MaterialLibrary
{
private:
    static MaterialLibrary *self;
    
    typedef std::map<std::string, Material &> material_map_t;
    material_map_t materials;
    
    
    MaterialLibrary();
    virtual ~MaterialLibrary();
    
public:
    static MaterialLibrary * inst();
    const Material & find(std::string name);

    static Material emerald;
    static Material jade;
    static Material obsidian;
    static Material pearl;
    static Material ruby;
    static Material turquoise;
    static Material brass;
    static Material bronze;
    static Material chrome;
    static Material copper;
    static Material gold;
    static Material silver;
    static Material black_plastic;
    static Material cyan_plastic;
    static Material green_plastic;
    static Material red_plastic;
    static Material white_plastic;
    static Material yellow_plastic;
    static Material black_rubber;
    static Material cyan_rubber;
    static Material green_rubber;
    static Material red_rubber;
    static Material white_rubber;
    static Material yellow_rubber;
};
