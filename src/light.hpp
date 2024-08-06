using namespace glm;
struct Light {
    vec4 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    Light(){}
    Light(vec4 ambient, vec3 diffuse, vec3 specular, vec3 position){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->position = position;
    }
    void updateUniform(GLuint shaderProgram){
    glUniform4fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1, value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1, value_ptr(position));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, value_ptr(specular));
}
};
struct dirLight : public Light {
    vec3 direction;
    dirLight(){}
    dirLight(vec4 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 direction){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->position = position;
        this->direction = direction;
    }
    void updateUniform(GLuint shaderProgram){
    glUniform4fv(glGetUniformLocation(shaderProgram, "light.ambient"), 1,value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.diffuse"), 1, value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.position"), 1,value_ptr(position) );
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.specular"), 1, value_ptr(specular));
    glUniform3fv(glGetUniformLocation(shaderProgram, "light.direction"), 1, value_ptr(direction));
}
};
struct Material {
    vec4 ambient;
    vec3 diffuse;
    vec3 specular;
    GLfloat shininess;
    Material(){}
    Material(vec4 ambient, vec3 diffuse, vec3 specular, GLfloat shininess){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }
    void updateUniform(GLuint shaderProgram){
    glUniform4fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1,value_ptr(this->ambient));
    glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, value_ptr(this->diffuse));
    glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, value_ptr(this->specular));
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), this->shininess);
}
};
