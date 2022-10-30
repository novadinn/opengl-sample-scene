#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <vector>
#include <string>

#include "raw_model.h"
#include "indexed_model.h"
#include "vertex.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "textures.h"
#include "shader.h"
#include "frame_buffer.h"

struct ResourceLoader {
    RawModel loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<float> tex_coords);
    RawModel loadToVAO(std::vector<float> positions, int dimension);
    IndexedModel setupMeshVAO(std::vector<Vertex> vertices, std::vector<uint> indices);
    
    Texture2D loadTexture(const char *file);
    Texture2D createTextureAttachment(int width, int height);
    Texture2D createDepthTextureAttachment(int width, int height);
    uint createDepthBufferAttachment(int width, int height);
    Texture3D loadCubeMap(std::vector<std::string> faces);

    FrameBuffer createFrameBuffer();
    
    Shader loadVSFSShader(const char *vs, const char *fs);
    Shader loadVSGSFSShader(const char *vs, const char* gs, const char *fs);
    
    void clear();
private:
    int createVAO();
    void storeDataInAttributeList(int attribute_number, int coordinate_size, std::vector<float> data);

    void checkShaderCompileErrors(uint object, std::string type);
    
    std::vector<uint> VAOs_;
    std::vector<uint> VBOs_;
    std::vector<uint> EBOs_;
    std::vector<uint> textures_;
    std::vector<uint> FBOs_;
    std::vector<uint> renderbuffers_;
    std::vector<uint> shaders_;
};

#endif
