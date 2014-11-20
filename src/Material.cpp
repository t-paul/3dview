#include "Material.h"

Material::Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    _name = name;
    _ambient = ambient;
    _diffuse = diffuse;
    _specular = specular;
    _shininess = shininess;
}

Material::~Material()
{
}

const std::string & Material::name() const
{
    return _name;
}

const glm::vec3 & Material::ambientColor() const
{
    return _ambient;
}

const glm::vec3 & Material::diffuseColor() const
{
    return _diffuse;
}

const glm::vec3 & Material::specularColor() const
{
    return _specular;
}

const float Material::shininess() const
{
    return _shininess;
}


MaterialLibrary * MaterialLibrary::self = NULL;

MaterialLibrary::MaterialLibrary()
{
    materials.insert(material_map_t::value_type(emerald.name(), emerald));
    materials.insert(material_map_t::value_type(jade.name(), jade));
    materials.insert(material_map_t::value_type(obsidian.name(), obsidian));
    materials.insert(material_map_t::value_type(pearl.name(), pearl));
    materials.insert(material_map_t::value_type(ruby.name(), ruby));
    materials.insert(material_map_t::value_type(turquoise.name(), turquoise));
    materials.insert(material_map_t::value_type(brass.name(), brass));
    materials.insert(material_map_t::value_type(bronze.name(), bronze));
    materials.insert(material_map_t::value_type(chrome.name(), chrome));
    materials.insert(material_map_t::value_type(copper.name(), copper));
    materials.insert(material_map_t::value_type(gold.name(), gold));
    materials.insert(material_map_t::value_type(silver.name(), silver));
    materials.insert(material_map_t::value_type(black_plastic.name(), black_plastic));
    materials.insert(material_map_t::value_type(cyan_plastic.name(), cyan_plastic));
    materials.insert(material_map_t::value_type(green_plastic.name(), green_plastic));
    materials.insert(material_map_t::value_type(red_plastic.name(), red_plastic));
    materials.insert(material_map_t::value_type(white_plastic.name(), white_plastic));
    materials.insert(material_map_t::value_type(yellow_plastic.name(), yellow_plastic));
    materials.insert(material_map_t::value_type(black_rubber.name(), black_rubber));
    materials.insert(material_map_t::value_type(cyan_rubber.name(), cyan_rubber));
    materials.insert(material_map_t::value_type(green_rubber.name(), green_rubber));
    materials.insert(material_map_t::value_type(red_rubber.name(), red_rubber));
    materials.insert(material_map_t::value_type(white_rubber.name(), white_rubber));
    materials.insert(material_map_t::value_type(yellow_rubber.name(), yellow_rubber));
}

MaterialLibrary::~MaterialLibrary()
{
}

MaterialLibrary *
MaterialLibrary::inst()
{
    if (self == NULL) {
	self = new MaterialLibrary();
    }
    return self;
}

const Material &
MaterialLibrary::find(std::string name)
{
    material_map_t::iterator it = materials.find(name);
    if (it != materials.end()) {
	return (*it).second;
    }
    
    return silver;
}
    
// see http://devernay.free.fr/cours/opengl/materials.html

Material MaterialLibrary::emerald = Material("Emerald",
	glm::vec3(0.0215, 0.1745, 0.0215),
	glm::vec3(0.07568, 0.61424, 0.07568),
	glm::vec3(0.633, 0.727811, 0.633),
	0.6);

Material MaterialLibrary::jade = Material("Jade",
	glm::vec3(0.135, 0.2225, 0.1575),
	glm::vec3(0.54, 0.89, 0.63),
	glm::vec3(0.316228, 0.316228, 0.316228),
	0.1);

Material MaterialLibrary::obsidian = Material("Obsidian",
	glm::vec3(0.05375, 0.05, 0.06625),
	glm::vec3(0.18275, 0.17, 0.22525),
	glm::vec3(0.332741, 0.328634, 0.346435),
	0.3);

Material MaterialLibrary::pearl = Material("Pearl",
	glm::vec3(0.25, 0.20725, 0.20725),
	glm::vec3(1, 0.829, 0.829),
	glm::vec3(0.296648, 0.296648, 0.296648),
	0.088);

Material MaterialLibrary::ruby = Material("Ruby",
	glm::vec3(0.1745, 0.01175, 0.01175),
	glm::vec3(0.61424, 0.04136, 0.04136),
	glm::vec3(0.727811, 0.626959, 0.626959),
	0.6);

Material MaterialLibrary::turquoise = Material("Turquoise",
	glm::vec3(0.1, 0.18725, 0.1745),
	glm::vec3(0.396, 0.74151, 0.69102),
	glm::vec3(0.297254, 0.30829, 0.306678),
	0.1);

Material MaterialLibrary::brass = Material("Brass",
	glm::vec3(0.329412, 0.223529, 0.027451),
	glm::vec3(0.780392, 0.568627, 0.113725),
	glm::vec3(0.992157, 0.941176, 0.807843),
	0.21794872);

Material MaterialLibrary::bronze = Material("Bronze",
	glm::vec3(0.2125, 0.1275, 0.054),
	glm::vec3(0.714, 0.4284, 0.18144),
	glm::vec3(0.393548, 0.271906, 0.166721),
	0.2);

Material MaterialLibrary::chrome = Material("Chrome",
	glm::vec3(0.25, 0.25, 0.25),
	glm::vec3(0.4, 0.4, 0.4),
	glm::vec3(0.774597, 0.774597, 0.774597),
	0.6);

Material MaterialLibrary::copper = Material("Copper",
	glm::vec3(0.19125, 0.0735, 0.0225),
	glm::vec3(0.7038, 0.27048, 0.0828),
	glm::vec3(0.256777, 0.137622, 0.086014),
	0.1);

Material MaterialLibrary::gold = Material("Gold",
	glm::vec3(0.24725, 0.1995, 0.0745),
	glm::vec3(0.75164, 0.60648, 0.22648),
	glm::vec3(0.628281, 0.555802, 0.366065),
	0.4);

Material MaterialLibrary::silver = Material("Silver",
	glm::vec3(0.19225, 0.19225, 0.19225),
	glm::vec3(0.50754, 0.50754, 0.50754),
	glm::vec3(0.508273, 0.508273, 0.508273),
	0.4);

Material MaterialLibrary::black_plastic = Material("Black Plastic",
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.01, 0.01, 0.01),
	glm::vec3(0.50, 0.50, 0.50),
	.25);

Material MaterialLibrary::cyan_plastic = Material("Cyan Plastic",
	glm::vec3(0.0, 0.1, 0.06),
	glm::vec3(0.0, 0.50980392, 0.50980392),
	glm::vec3(0.50196078, 0.50196078, 0.50196078),
	.25);

Material MaterialLibrary::green_plastic = Material("Green Plastic",
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.1, 0.35, 0.1),
	glm::vec3(0.45, 0.55, 0.45),
	.25);

Material MaterialLibrary::red_plastic = Material("Red Plastic",
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.5, 0.0, 0.0),
	glm::vec3(0.7, 0.6, 0.6),
	.25);

Material MaterialLibrary::white_plastic = Material("White Plastic",
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.55, 0.55, 0.55),
	glm::vec3(0.70, 0.70, 0.70),
	.25);

Material MaterialLibrary::yellow_plastic = Material("Yellow Plastic",
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.5, 0.5, 0.0),
	glm::vec3(0.60, 0.60, 0.50),
	.25);

Material MaterialLibrary::black_rubber = Material("Black Rubber",
	glm::vec3(0.02, 0.02, 0.02),
	glm::vec3(0.01, 0.01, 0.01),
	glm::vec3(0.4, 0.4, 0.4),
	.078125);

Material MaterialLibrary::cyan_rubber = Material("Cyan Rubber",
	glm::vec3(0.0, 0.05, 0.05),
	glm::vec3(0.4, 0.5, 0.5),
	glm::vec3(0.04, 0.7, 0.7),
	.078125);

Material MaterialLibrary::green_rubber = Material("Green Rubber",
	glm::vec3(0.0, 0.05, 0.0),
	glm::vec3(0.4, 0.5, 0.4),
	glm::vec3(0.04, 0.7, 0.04),
	.078125);

Material MaterialLibrary::red_rubber = Material("Red Rubber",
	glm::vec3(0.05, 0.0, 0.0),
	glm::vec3(0.5, 0.4, 0.4),
	glm::vec3(0.7, 0.04, 0.04),
	.078125);

Material MaterialLibrary::white_rubber = Material("White Rubber",
	glm::vec3(0.05, 0.05, 0.05),
	glm::vec3(0.5, 0.5, 0.5),
	glm::vec3(0.7, 0.7, 0.7),
	.078125);

Material MaterialLibrary::yellow_rubber = Material("Yellow Rubber",
	glm::vec3(0.05, 0.05, 0.0),
	glm::vec3(0.5, 0.5, 0.4),
	glm::vec3(0.7, 0.7, 0.04),
	.078125);


	