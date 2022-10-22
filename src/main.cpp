#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/resource_loader.cpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.cpp"
#include "graphics/raw_model.cpp"
#include "graphics/shader.cpp"
#include "graphics/textures.cpp"
#include "graphics/directional_light.cpp"
#include "graphics/spot_light.cpp"
#include "objects/game_object.cpp"
#include "objects/cube_map.cpp"
#include "platform.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

internal void framebuffer_size_callback(GLFWwindow* window, int width, int height);
internal void process_input(GLFWwindow* window);
internal void mouse_callback(GLFWwindow* window, double xpos, double ypos);
internal void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

global_variable Camera global_camera(glm::vec3(0.0f, 0.0f, 3.0f));
global_variable float global_last_x = WINDOW_WIDTH / 2.0f;
global_variable float global_last_y = WINDOW_HEIGHT / 2.0f;
global_variable bool global_first_mouse = true;

global_variable float global_delta_time = 0.0f;
global_variable float global_last_time = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    std::vector<float> cube_positions = {
	-0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f,  
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f 
    };
    std::vector<float> cube_normals = {
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  
	0.0f,  0.0f, -1.0f,  

	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  
	0.0f,  0.0f,  1.0f,  

	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	-1.0f,  0.0f,  0.0f,  
	
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	1.0f,  0.0f,  0.0f,  
	
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	0.0f, -1.0f,  0.0f,  
	
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f,  
	0.0f,  1.0f,  0.0f
    };
    std::vector<float> cube_tex_coords = {
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	
	0.0f,  0.0f,
	1.0f,  0.0f,
	1.0f,  1.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	
	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
	
	1.0f,  0.0f,
	1.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  1.0f,
	0.0f,  0.0f,
	1.0f,  0.0f,
	
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	1.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  1.0f,
	
	0.0f,  1.0f,
	1.0f,  1.0f,
	1.0f,  0.0f,
	1.0f,  0.0f,
	0.0f,  0.0f,
	0.0f,  1.0f
    };

    ResourceLoader loader;
    RawModel cube_model = loader.loadToVAO(cube_positions, cube_normals, cube_tex_coords);    
    // TODO: create file_system
    Texture2D container_texture = loader.loadTexture("img\\textures\\container2.png");
    Texture2D container_specular_texture = loader.loadTexture
	("img\\textures\\container2_specular.png");
    Shader shader = loader.loadShader("shaders\\default.vs", "shaders\\default.fs");
    shader.bind();
    shader.setInteger("material.diffuse", 0);
    shader.setInteger("material.specular", 1); // TODO: bind that in game object class
    shader.unbind();
    GameObject cube(cube_model, shader, container_texture);
    cube.position = glm::vec3(3.0f, -1.0f, 0.0f);
    
    std::vector<float> skybox_vertices = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
    };

    std::vector<std::string> day_faces {
	"img\\textures\\skybox\\right.jpg",
	"img\\textures\\skybox\\left.jpg",
	"img\\textures\\skybox\\top.jpg",
	"img\\textures\\skybox\\bottom.jpg",
	"img\\textures\\skybox\\front.jpg",
	"img\\textures\\skybox\\back.jpg"
    };
    std::vector<std::string> night_faces {
	"img\\textures\\skybox\\nightRight.jpg",
	"img\\textures\\skybox\\nightLeft.jpg",
	"img\\textures\\skybox\\nightTop.jpg",
	"img\\textures\\skybox\\nightBottom.jpg",
	"img\\textures\\skybox\\nightFront.jpg",
	"img\\textures\\skybox\\nightBack.jpg"
    };
    Texture3D skybox_day_texture = loader.loadCubeMap(day_faces);
    Texture3D skybox_night_texture = loader.loadCubeMap(night_faces);
    RawModel cube_map_model = loader.loadToVAO(skybox_vertices);
    Shader skybox_shader = loader.loadShader("shaders\\skybox.vs", "shaders\\skybox.fs");
    CubeMap cube_map(cube_map_model, skybox_shader, skybox_day_texture, skybox_night_texture);

    DirectionalLight dir_light(shader, glm::vec3(-0.2f, -1.0f, -0.3f));
    SpotLight spot_light(shader);
    
    while (!glfwWindowShouldClose(window)) {
        float current_time = (float)glfwGetTime();
        global_delta_time = current_time - global_last_time;
        global_last_time = current_time;

	// update
        process_input(window);

	cube_map.update(global_delta_time);
	
	// draw
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = global_camera.getProjectionMatrix((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
								 0.1f, 100.0f);
        glm::mat4 view = global_camera.getViewMatrix();

	spot_light.position = global_camera.position;
	spot_light.direction = global_camera.front;
	
	shader.bind();
        shader.setFloat("material.shininess", 32.0f);

        dir_light.draw(global_camera.position);
	spot_light.draw(global_camera.position);
	
	cube.draw(projection, view);

	view = glm::mat4(glm::mat3(global_camera.getViewMatrix()));
	cube_map.draw(projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    loader.clear();

    glfwTerminate();
    return 0;
}

internal void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

internal void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) global_camera.moveForward(global_delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) global_camera.moveBackward(global_delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) global_camera.moveLeft(global_delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) global_camera.moveRight(global_delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) global_camera.moveUp(global_delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) global_camera.moveDown(global_delta_time);
}

internal void mouse_callback(GLFWwindow* window, double new_x, double new_y) {
    float x = static_cast<float>(new_x);
    float y = static_cast<float>(new_y);

    if (global_first_mouse) {
        global_last_x = x;
        global_last_y = y;
        global_first_mouse = false;
    }

    float x_offset = x - global_last_x;
    float y_offset = global_last_y - y;

    global_last_x = x;
    global_last_y = y;

    global_camera.processMouseMovement(x_offset, y_offset);
}

internal void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    global_camera.processMouseScroll(static_cast<float>(y_offset));
}
