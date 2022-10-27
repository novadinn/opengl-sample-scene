#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/type_ptr.hpp"
#include "graphics/resource_loader.cpp"
#include "graphics/raw_model.cpp"
#include "graphics/shader.cpp"
#include "graphics/textures.cpp"
#include "graphics/frame_buffer.cpp"
#include "graphics/directional_light.cpp"
#include "graphics/spot_light.cpp"
#include "graphics/point_light.cpp"
#include "objects/gui_element.cpp"
#include "objects/game_object.cpp"
#include "objects/textured_object.cpp"
#include "objects/cube_map.cpp"
#include "objects/flashlight.cpp"
#include "objects/water.cpp"
#include "camera.cpp"
#include "platform.h"
#include "primitives.h"
#include "file_system.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

internal void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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

    ResourceLoader loader;
    
    Texture2D container_diffuse_texture = loader.loadTexture(
	file_system::join("img\\textures\\container2.png").c_str());
    Texture2D container_specular_texture = loader.loadTexture(
	file_system::join("img\\textures\\container2_specular.png").c_str());

    RawModel cube_model =
	loader.loadToVAO(primitives::cube_positions, primitives::cube_normals, primitives::cube_tex_coords);
    RawModel simple_cube_model =
	loader.loadToVAO(primitives::cube_positions, 3);
    RawModel plane_model =
	loader.loadToVAO(primitives::plane_positions, primitives::plane_normals, primitives::plane_tex_coords);
    
    Shader main_shader = loader.loadVSFSShader(
	file_system::join("shaders\\main.vs").c_str(),
	file_system::join("shaders\\main.fs").c_str());
    Shader default_shader = loader.loadVSFSShader(
	file_system::join("shaders\\default.vs").c_str(),
	file_system::join("shaders\\default.fs").c_str());
    
    default_shader.bind();
    default_shader.setVector4f("color", glm::vec4(1.0f));
    Shader::unbind();
    
    DirectionalLight dir_light(glm::vec3(-0.2f, -1.0f, -0.3f));
    PointLight point_light(glm::vec3(4.0f, 0.5f, 0.0f), 0);
    
    Flashlight flashlight;
    TexturedObject cube(cube_model, main_shader, container_diffuse_texture, container_specular_texture);
    cube.position = glm::vec3(3.0f, -1.0f, 0.0f);
    TexturedObject plane(plane_model, main_shader, container_diffuse_texture, container_specular_texture);
    plane.position = glm::vec3(0.0f, -2.0f, 0.0f);
    CubeMap cube_map(loader);
    Water water(loader);
    water.position = glm::vec3(0.0f, -0.5f, 0.0f);

    GuiElement refraction(water.getRefractionTexture(), loader);
    refraction.position = glm::vec2(-0.5f, 0.5f);
    refraction.size = glm::vec2(0.25f, 0.25f);
    GuiElement reflection(water.getReflectionTexture(), loader);
    reflection.position = glm::vec2(0.5f, 0.5f);
    reflection.size = glm::vec2(0.25f, 0.25f);
    
    const glm::vec4 up_clip_plane(0.0f, -1.0f, 0.0f, water.position.y);
    // TODO: add those to all shaders (except water)!
    const glm::vec4 down_clip_plane(0.0f, 1.0f, 0.0f, -water.position.y);
    
    while (!glfwWindowShouldClose(window)) {
        float current_time = (float)glfwGetTime();
        global_delta_time = current_time - global_last_time;
        global_last_time = current_time;

	// update
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) global_camera.moveForward(global_delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) global_camera.moveBackward(global_delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) global_camera.moveLeft(global_delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) global_camera.moveRight(global_delta_time);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) global_camera.moveUp(global_delta_time);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) global_camera.moveDown(global_delta_time);
	// TODO: create a keyboard class and check for key up messages for that method
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) flashlight.toggle();

	cube_map.update(global_delta_time);
	flashlight.update(global_camera.position, global_camera.front);
	
	// draw
	auto render = [&]() {
	    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	    glm::mat4 projection = global_camera.getProjectionMatrix((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
								 0.1f, 100.0f);
	    glm::mat4 view = global_camera.getViewMatrix();
	    
	    dir_light.draw(main_shader, global_camera.position);
	    point_light.draw(main_shader, global_camera.position);
	    
	    flashlight.draw(main_shader, global_camera.position);
	    cube.draw(projection, view);
	    plane.draw(projection, view);

	    view = glm::mat4(glm::mat3(global_camera.getViewMatrix()));
	    cube_map.draw(projection, view);
	};

	glEnable(GL_CLIP_DISTANCE0);

	// reflection
	water.bindReflectionFrameBuffer();
	float distance = 2 * (global_camera.position.y - water.position.y);
	global_camera.position.y -= distance;
	global_camera.pitch = -global_camera.pitch;
	main_shader.bind();
	main_shader.setVector4f("plane", down_clip_plane);
	Shader::unbind();
	render();
	global_camera.position.y += distance;
	global_camera.pitch = -global_camera.pitch;

	// refraction
	water.bindRefractionFrameBuffer();
	main_shader.bind();
	main_shader.setVector4f("plane", up_clip_plane);
	Shader::unbind();
	render();
	glDisable(GL_CLIP_DISTANCE0); // FIXME: this works not for all drives for some reason, but you can
	// set the clip plane's height to some high value instead
	water.unbindCurrentFrameBuffer();

	render();
	glm::mat4 projection = global_camera.getProjectionMatrix((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
								 0.1f, 100.0f);
	glm::mat4 view = global_camera.getViewMatrix();
	water.draw(projection, view);
	refraction.draw();
	reflection.draw();
	
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
