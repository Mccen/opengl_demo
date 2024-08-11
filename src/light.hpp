struct Light
{
    glm::vec4 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
    Light() {}
    Light(Light &light, glm::vec4 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->position = position;
        light = *this;
    }
    void updateUniform(GLuint shaderProgram)
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, value_ptr(this->ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, value_ptr(this->diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, value_ptr(this->position));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, value_ptr(this->specular));
    }
};
struct dirLight : public Light
{
    glm::vec3 direction;
    dirLight() {}
    dirLight(glm::vec4 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->position = position;
        this->direction = direction;
    }
    void updateUniform(GLuint shaderProgram)
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, value_ptr(this->ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, value_ptr(this->diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, value_ptr(this->position));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, value_ptr(this->specular));
        glUniform3fv(glGetUniformLocation(shaderProgram, "light.direction"), 1, value_ptr(this->direction));
    }
};
struct Material
{
    glm::vec4 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    Material() {}
    Material(Material &material, glm::vec4 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
        material = *this;
    }

    void updateUniform(GLuint shaderProgram)
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, value_ptr(this->ambient));
        glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, value_ptr(this->diffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, value_ptr(this->specular));
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), this->shininess);
    }
};
