#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../platform.h"

struct Shader {
    Shader(uint id);
    
    void bind();
    shared void unbind();
    
    void setFloat(const char *name, float value);
    void setInteger(const char *name, int value);
    void setVector2f(const char *name, float x, float y);
    void setVector2f(const char *name, const glm::vec2 &value);
    void setVector3f(const char *name, float x, float y, float z);
    void setVector3f(const char *name, const glm::vec3 &value);
    void setVector4f(const char *name, float x, float y, float z, float w);
    void setVector4f(const char *name, const glm::vec4 &value);
    void setMatrix4(const char *name, const glm::mat4 &matrix);

    uint getId() const;
    
private:
    uint id_;
};

#endif
