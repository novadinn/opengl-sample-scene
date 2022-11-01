#include "textured_object.h"

TexturedObject::TexturedObject(RawModel model, Shader shader, std::vector<ObjectTexture> textures) :
    GameObject(shader), textures_(textures), model_(model) {
    
    shader_.bind();
    unsigned int n_diffuse = 1;
    unsigned int n_specular = 1;
    // unsigned int n_normal = 1;
    // unsigned int n_height = 1;
    for(int i = 0; i < textures_.size(); ++i) {
	std::string number;
	std::string name;
	if(textures_[i].type == DIFFUSE) {
	    number = std::to_string(n_diffuse++);
	    name = "material.diffuse";
	} else if(textures_[i].type == SPECULAR) {
	    number = std::to_string(n_specular++);
	    name = "material.specular";
	}
	// else if(name == "texture_normal")
	//     number = std::to_string(n_normal++);
	//  else if(name == "texture_height")
	//     number = std::to_string(n_height++);

	shader.setInteger((name+number).c_str(), i);
    }
    
    shader_.setFloat("material.shininess", 32.0f);
    Shader::unbind();
}

void TexturedObject::draw(glm::mat4& projection, glm::mat4& view,
			  glm::vec3& view_pos, DirectionalLight& dir_light,
			  SpotLight& spot_light, PointLight& point_light) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
    model = glm::scale(model, size);

    shader_.bind();
    for(int i = 0; i < textures_.size(); ++i) {
	Texture2D::activate(i);
	textures_[i].texture.bind();
    }
    
    model_.bind();
    RawModel::enableAttribute(0);
    RawModel::enableAttribute(1);
    RawModel::enableAttribute(2);

    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", view);
    shader_.setMatrix4("model", model);

    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f("dirLight.direction", dir_light.direction);
    shader_.setVector3f("dirLight.ambient", dir_light.ambient);
    shader_.setVector3f("dirLight.diffuse", dir_light.diffuse);
    shader_.setVector3f("dirLight.specular", dir_light.specular);

    shader_.setVector3f("spotLight.position", spot_light.position);
    shader_.setVector3f("spotLight.direction", spot_light.direction);
    shader_.setVector3f("spotLight.ambient", spot_light.ambient);
    shader_.setVector3f("spotLight.diffuse", spot_light.diffuse);
    shader_.setVector3f("spotLight.specular", spot_light.specular);
    shader_.setFloat("spotLight.constant", spot_light.constant);
    shader_.setFloat("spotLight.linear", spot_light.linear);
    shader_.setFloat("spotLight.quadratic", spot_light.quadratic);
    shader_.setFloat("spotLight.cutOff", spot_light.cutoff);
    shader_.setFloat("spotLight.outerCutOff", spot_light.outer_cutoff);
    
    std::string number = std::to_string(point_light.index);
    shader_.setVector3f(("pointLights[" + number + "].position").c_str(), point_light.position);
    shader_.setVector3f(("pointLights[" + number + "].ambient").c_str(), point_light.ambient);
    shader_.setVector3f(("pointLights[" + number + "].diffuse").c_str(), point_light.diffuse);
    shader_.setVector3f(("pointLights[" + number + "].specular").c_str(), point_light.specular);
    shader_.setFloat(("pointLights[" + number + "].constant").c_str(), point_light.constant);
    shader_.setFloat(("pointLights[" + number + "].linear").c_str(), point_light.linear);
    shader_.setFloat(("pointLights[" + number + "].quadratic").c_str(), point_light.quadratic);
    
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());

    RawModel::disableAttribute(0);
    RawModel::disableAttribute(1);
    RawModel::disableAttribute(2);
    RawModel::unbind();
    Texture2D::deactivate();
    Texture2D::unbind();
    Shader::unbind();
}
