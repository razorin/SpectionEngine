#include "Material.h"



Material::Material()
{
}

Material::Material(const Material * material) : diffuse(material->diffuse), specular(material->specular),
ambient(material->ambient), emissive(material->emissive), transparent(material->transparent), shininess(material->shininess)
{
}


Material::~Material()
{
}
