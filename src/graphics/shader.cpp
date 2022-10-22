#include "shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(uint id) : id_(id) {}

void Shader::bind() {
    glUseProgram(id_);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::setFloat(const char *name, float value) {
    glUniform1f(glGetUniformLocation(id_, name), value);
}
void Shader::setInteger(const char *name, int value) {
    glUniform1i(glGetUniformLocation(id_, name), value);
}
void Shader::setVector2f(const char *name, float x, float y) {
    glUniform2f(glGetUniformLocation(id_, name), x, y);
}
void Shader::setVector2f(const char *name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}
void Shader::setVector3f(const char *name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}
void Shader::setVector3f(const char *name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const char *name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}
void Shader::setVector4f(const char *name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const char *name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false, glm::value_ptr(matrix));
}

uint Shader::getId() const {
    return id_;
}
