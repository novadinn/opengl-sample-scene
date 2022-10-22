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

RawModel ResourceLoader::loadToVAO(std::vector<float> positions) {
    uint VAO_id = createVAO();
    storeDataInAttributeList(0, 3, positions);
    glBindVertexArray(0);
    return RawModel(VAO_id, positions.size()/3);
}

Texture2D ResourceLoader::loadTexture(const char *file) {
    uint texture_id;
    glGenTextures(1, &texture_id);

    int width, height, n_channels;
    uchar* data = stbi_load(file, &width, &height, &n_channels, 0);

    uint internal_format = (n_channels == 4) ? GL_RGBA : GL_RGB;
    uint image_format = (n_channels == 4) ? GL_RGBA : GL_RGB;
    // TODO: create a method that can take those as arguments
    uint wrap_s = GL_REPEAT;
    uint wrap_t = GL_REPEAT;
    uint filter_min = GL_LINEAR;
    uint filter_max = GL_LINEAR;
    
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(data);

    textures_.push_back(texture_id);
    return Texture2D(texture_id, width, height);
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

Shader ResourceLoader::loadShader(const char *vs, const char *fs) {
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

void ResourceLoader::clear() {
    for(int i = 0; i < VAOs_.size(); ++i) {
	glDeleteVertexArrays(1, &VAOs_[i]);
    }
    VAOs_.clear();
    for(int i = 0; i < VBOs_.size(); ++i) {
	glDeleteBuffers(1, &VBOs_[i]);
    }
    VBOs_.clear();
    for(int i = 0; i < textures_.size(); ++i) {
	glDeleteTextures(1, &textures_[i]);
    }
    textures_.clear();
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
    glEnableVertexAttribArray(attribute_number);
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
