#include "light.hpp"

Light::Light() {}
Light::Light(Light &light, glm::vec4 ambient, glm::vec3 diffuse,
             glm::vec3 specular, glm::vec3 position, glm::vec3 direction,
             glm::vec3 attenuation) {
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->position = position;
  this->direction = direction;
  this->attenuation = attenuation;
  light = *this;
}
void Light::updateUniform(GLuint shaderProgram) {
  glUniform4fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1,
               value_ptr(this->ambient));
  glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1,
               value_ptr(this->diffuse));
  glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1,
               value_ptr(this->specular));
  glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1,
               value_ptr(this->position));
  glUniform3fv(glGetUniformLocation(shaderProgram, "light.direction"), 1,
               value_ptr(this->direction));
  glUniform3fv(glGetUniformLocation(shaderProgram, "light.attenuation"), 1,
               value_ptr(this->attenuation));
}
Material::Material() {}
Material::Material(Material &material, glm::vec4 ambient, glm::vec3 diffuse,
                   glm::vec3 specular, GLfloat shininess) {
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shininess = shininess;
  material = *this;
}

void Material::updateUniform(GLuint shaderProgram) {
  glUniform4fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1,
               value_ptr(this->ambient));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1,
               value_ptr(this->diffuse));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1,
               value_ptr(this->specular));
  glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"),
              this->shininess);
}