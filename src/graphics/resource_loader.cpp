#include "resource_loader.h"

#include <vector>
#include <sstream>
#include <fstream>

RawModel ResourceLoader::loadToVAO(std::vector<float> positions,
				   std::vector<float> normals, std::vector<float> tex_coords) {
    uint VAO_id = createVAO();
    storeDataInAttributeList(0, 3, positions);
    storeDataInAttributeList(1, 3, normals);
    storeDataInAttributeList(2, 2, tex_coords);
    glBindVertexArray(0);
    return RawModel(VAO_id, positions.size()/3);
}

RawModel ResourceLoader::loadToVAO(std::vector<float> positions, int dimension) {
    uint VAO_id = createVAO();
    storeDataInAttributeList(0, dimension, positions);
    glBindVertexArray(0);
    return RawModel(VAO_id, positions.size()/dimension);
}

RawModel ResourceLoader::setupMeshVAO(std::vector<Vertex> vertices, std::vector<uint> indices) {
    uint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	
    glBindVertexArray(0);

    VAOs_.push_back(VAO);
    VBOs_.push_back(VBO);
    EBOs_.push_back(EBO);
    return RawModel(VAO, indices.size());
}

Texture2D ResourceLoader::loadTexture(const char *file) {
    return loadTexture(file, TextureData());
}

Texture2D ResourceLoader::loadTexture(const char *file, TextureData texture_data) {
    uint texture_id;
    glGenTextures(1, &texture_id);

    int width, height, n_channels;
    uchar* data = stbi_load(file, &width, &height, &n_channels, 0);

    uint internal_format = (n_channels == 4) ? GL_RGBA : GL_RGB;
    uint image_format = (n_channels == 4) ? GL_RGBA : GL_RGB;
    
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_data.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_data.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_data.filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_data.filter_max);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(data);

    textures_.push_back(texture_id);
    return Texture2D(texture_id, width, height);
}

Texture2D ResourceLoader::createTextureAttachment(int width, int height) {
    uint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);
    
    textures_.push_back(id);
    return Texture2D(id, width, height);
}

Texture2D ResourceLoader::createDepthTextureAttachment(int width, int height) {
    uint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);

    textures_.push_back(id);
    return Texture2D(id, width, height);
}

uint ResourceLoader::createDepthBufferAttachment(int width, int height) {
    uint id;
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);

    renderbuffers_.push_back(id);
    return id;
}

Texture3D ResourceLoader::loadCubeMap(std::vector<std::string> faces) {
    uint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, n_channels;
    for (uint i = 0; i < faces.size(); i++) {
        uchar *data = stbi_load(faces[i].c_str(), &width, &height, &n_channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
			 GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textures_.push_back(texture_id);
    return Texture3D(texture_id);
}

FrameBuffer ResourceLoader::createFrameBuffer() {
    uint id;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return FrameBuffer(id);
}

Shader ResourceLoader::loadVSFSShader(const char *vs, const char *fs) {
    std::string v_code;
    std::string f_code;
    try {
        std::ifstream vs_file(vs);
        std::ifstream fs_file(fs);
        std::stringstream vs_stream, fs_stream;
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();
        vs_file.close();
        fs_file.close();
        v_code = vs_stream.str();
        f_code = fs_stream.str();
    } catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vs_code = v_code.c_str();
    const char *fs_code = f_code.c_str();
    
    unsigned int vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_code, NULL);
    glCompileShader(vertex_shader);
    checkShaderCompileErrors(vertex_shader, "VERTEX");
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_code, NULL);
    glCompileShader(fragment_shader);
    checkShaderCompileErrors(fragment_shader, "FRAGMENT");

    uint shader_id;
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, fragment_shader);
    glLinkProgram(shader_id);
    checkShaderCompileErrors(shader_id, "PROGRAM");
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    shaders_.push_back(shader_id);
    return Shader(shader_id);
}

Shader ResourceLoader::loadVSGSFSShader(const char *vs, const char* gs, const char *fs) {
    std::string v_code;
    std::string g_code;
    std::string f_code;
    try {
        std::ifstream vs_file(vs);
	std::ifstream gs_file(gs);
        std::ifstream fs_file(fs);
        std::stringstream vs_stream, gs_stream, fs_stream;
        vs_stream << vs_file.rdbuf();
	gs_stream << gs_file.rdbuf();
        fs_stream << fs_file.rdbuf();
        vs_file.close();
	gs_file.close();
        fs_file.close();
        v_code = vs_stream.str();
	g_code = gs_stream.str();
        f_code = fs_stream.str();
    } catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vs_code = v_code.c_str();
    const char *gs_code = g_code.c_str();
    const char *fs_code = f_code.c_str();
    
    unsigned int vertex_shader, geometry_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs_code, NULL);
    glCompileShader(vertex_shader);
    checkShaderCompileErrors(vertex_shader, "VERTEX");

    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry_shader, 1, &gs_code, NULL);
    glCompileShader(geometry_shader);
    checkShaderCompileErrors(geometry_shader, "GEOMETRY");
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs_code, NULL);
    glCompileShader(fragment_shader);
    checkShaderCompileErrors(fragment_shader, "FRAGMENT");

    uint shader_id;
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, geometry_shader);
    glAttachShader(shader_id, fragment_shader);
    glLinkProgram(shader_id);
    checkShaderCompileErrors(shader_id, "PROGRAM");
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    shaders_.push_back(shader_id);
    return Shader(shader_id);
}

void ResourceLoader::clear() {
    for(int i = 0; i < VAOs_.size(); ++i) {
	glDeleteVertexArrays(1, &VAOs_[i]);
    }
    VAOs_.clear();
    for(int i = 0; i < VBOs_.size(); ++i) {
	glDeleteBuffers(1, &VBOs_[i]);
    }
    VBOs_.clear();
    for(int i = 0; i < EBOs_.size(); ++i) {
	glDeleteBuffers(1, &EBOs_[i]);
    }
    EBOs_.clear();
    for(int i = 0; i < textures_.size(); ++i) {
	glDeleteTextures(1, &textures_[i]);
    }
    textures_.clear();
    for(int i = 0; i < FBOs_.size(); ++i) {
	glDeleteFramebuffers(1, &FBOs_[i]);
    }
    FBOs_.clear();
    for(int i = 0; i < renderbuffers_.size(); ++i) {
	glDeleteRenderbuffers(1, &renderbuffers_[i]);
    }
    renderbuffers_.clear();
    for(int i = 0; i < shaders_.size(); ++i) {
	glDeleteProgram(shaders_[i]);
    }
    shaders_.clear();
}

int ResourceLoader::createVAO() {
    uint VAO_id;
    glGenVertexArrays(1, &VAO_id);
    VAOs_.push_back(VAO_id);
    glBindVertexArray(VAO_id);
    return VAO_id;
}

void ResourceLoader::storeDataInAttributeList(int attribute_number, int coordinate_size, std::vector<float> data) {
    uint VBO_id;
    glGenBuffers(1, &VBO_id);
    VBOs_.push_back(VBO_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attribute_number, coordinate_size, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(attribute_number);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ResourceLoader::checkShaderCompileErrors(uint object, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
		      << infoLog << "\n -- --------------------------------------------------- -- "
		      << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
		      << infoLog << "\n -- --------------------------------------------------- -- "
		      << std::endl;
        }
    }
}
