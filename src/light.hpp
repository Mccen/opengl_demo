#include"headerLists.hpp"
struct Light
{
    glm::vec4 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
    glm::vec3 direction;
    //衰减系数
    glm::vec3 attenuation;
    Light();
    Light(Light &light, glm::vec4 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction,glm::vec3 attenuation);
    void updateUniform(GLuint shaderProgram);
};
struct Material
{
    glm::vec4 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    Material();
    Material(Material &material, glm::vec4 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);

    void updateUniform(GLuint shaderProgram);
};
