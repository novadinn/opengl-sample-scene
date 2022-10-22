#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <vector>
#include <string>

#include "raw_model.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "textures.h"
#include "shader.h"

struct ResourceLoader {
    RawModel loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<float> tex_coords);
    RawModel loadToVAO(std::vector<float> positions);
    Texture2D loadTexture(const char *file);
    Texture3D loadCubeMap(std::vector<std::string> faces);
    Shader loadShader(const char *vs, const char *fs);
    
    void clear();
private:
    int createVAO();
    void storeDataInAttributeList(int attribute_number, int coordinate_size, std::vector<float> data);

    void checkShaderCompileErrors(uint object, std::string type);
    
    std::vector<uint> VAOs_;
    std::vector<uint> VBOs_;
    std::vector<uint> textures_;
    std::vector<uint> shaders_;
};

#endif
