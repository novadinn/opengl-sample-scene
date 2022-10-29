#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // glm::vec3 Tangent;
    // glm::vec3 Bitangent;
};

// TODO: change to ObjectTexture
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh {

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
	vertices_(vertices), indices_(indices), textures_(textures) {

        setupMesh();
    }

    void draw(Shader &shader) {
	uint n_diffuse = 1;
	uint n_specular = 1;
	// unsigned int normalNr = 1;
	// unsigned int heightNr = 1;
	for(int i = 0; i < textures_.size(); i++) {
	    Texture2D::activate(i);
	    std::string number;
	    std::string name = textures_[i].type;
	    if(name == "texture_diffuse")
		number = std::to_string(n_diffuse++);
	    else if(name == "texture_specular")
		number = std::to_string(n_specular++);
	    // else if(name == "texture_normal")
	    // 	number = std::to_string(normalNr++);
	    // else if(name == "texture_height")
	    // 	number = std::to_string(heightNr++);

	    shader.setInteger((name+number).c_str(), i);
	    glBindTexture(GL_TEXTURE_2D, textures_[i].id); // textures_[i].bind();
	}
        
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, (indices_.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	Texture2D::deactivate();
    }

private:
    void setupMesh() {
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);  

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// glEnableVertexAttribArray(3);
	// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	
	// glEnableVertexAttribArray(4);
	// glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
    }

    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;
    // TODO: add indexed_model
    uint VAO_, VBO_, EBO_;
};

#endif
