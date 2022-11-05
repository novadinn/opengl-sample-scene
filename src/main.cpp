#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/type_ptr.hpp"

#include "graphics/resource_loader.cpp"
#include "graphics/raw_model.cpp"
#include "graphics/shader.cpp"
#include "graphics/textures.cpp"
#include "graphics/frame_buffer.cpp"
#include "graphics/mesh.cpp"
#include "graphics/model.cpp"

#include "objects/directional_light.cpp"
#include "objects/spot_light.cpp"
#include "objects/point_light.cpp"
#include "objects/gui_element.cpp"
#include "objects/game_object.cpp"
#include "objects/textured_object.cpp"
#include "objects/cube_map.cpp"
#include "objects/flashlight.cpp"
#include "objects/water.cpp"
#include "objects/grass.cpp"
#include "objects/shaded_model.cpp"

#include "camera.cpp"
#include "keyboard.cpp"
#include "display.cpp"
#include "platform.h"
#include "primitives.h"
#include "file_system.h"
#include "error_logging.h"

internal void framebuffer_size_callback(GLFWwindow* window, int width, int height);
internal void mouse_callback(GLFWwindow* window, double xpos, double ypos);
internal void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

global_variable Camera global_camera(glm::vec3(0.0f, 0.0f, 3.0f));
global_variable float global_last_x = Display::kWindowWidth / 2.0f;
global_variable float global_last_y = Display::kWindowHeight / 2.0f;
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

    GLFWwindow* window = glfwCreateWindow(Display::kWindowWidth, Display::kWindowHeight, "Scene", NULL, NULL);
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

    GL_CHECK(glEnable(GL_DEPTH_TEST));

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

    Shader color_shader = loader.loadVSFSShader(
	file_system::join("shaders\\colored_vert.glsl").c_str(),
	file_system::join("shaders\\colored_frag.glsl").c_str());
    
    Flashlight flashlight;
    
    DirectionalLight dir_light(glm::vec3(-2000.0f, -100000.0f, -3000.0f));
    PointLight point_light(glm::vec3(-0.07f, 1.5f, 2.0f), 0);
    SpotLight* spot_light(&flashlight.spot_light);

    std::vector<ObjectTexture> object_textures = {
	{ DIFFUSE, container_diffuse_texture },
	{ SPECULAR, container_specular_texture }
    };
    CubeMap cube_map(loader);
    Water water(loader);
    water.position = glm::vec3(0.0f, -0.45f, 0.0f);
    water.size = glm::vec3(4.0f);

    ShadedModel model(loader, color_shader, file_system::join("objects\\garden.obj"));

    Grass grass(loader);
    grass.position = glm::vec3(4.0f, -0.37f, -2.6f);
    Grass grass2(loader);
    grass2.position = glm::vec3(-4.0f, -0.37f, -2.85f);
    Grass grass3(loader);
    grass3.position = glm::vec3(-3.7f, -0.37f, 2.2f);
    grass3.rotation = glm::vec3(0.0f, 90.0f, 0.0f);
    Grass grass4(loader);
    grass4.position = glm::vec3(0.6f, -0.37f, 4.0f);
    grass4.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
    
    const glm::vec4 up_clip_plane(0.0f, -1.0f, 0.0f, water.position.y);
    // TODO: add those to all shaders (except water)!
    const glm::vec4 down_clip_plane(0.0f, 1.0f, 0.0f, -water.position.y);
    const glm::vec4 high_clip_plane(1.0f, -1.0f, 0.0f, (float)MAX_INTEGER);

    Keyboard keyboard;
    keyboard.addKeyBinding(GLFW_KEY_ESCAPE);
    keyboard.addKeyBinding(GLFW_KEY_W);
    keyboard.addKeyBinding(GLFW_KEY_S);
    keyboard.addKeyBinding(GLFW_KEY_A);
    keyboard.addKeyBinding(GLFW_KEY_D);
    keyboard.addKeyBinding(GLFW_KEY_SPACE);
    keyboard.addKeyBinding(GLFW_KEY_LEFT_CONTROL);
    keyboard.addKeyBinding(GLFW_KEY_F);
        
    while (!glfwWindowShouldClose(window)) {
        float current_time = (float)glfwGetTime();
        global_delta_time = current_time - global_last_time;
        global_last_time = current_time;

	// update
	keyboard.updateKeyStates(window);

	grass.update(global_delta_time);
	grass2.update(global_delta_time);
	grass3.update(global_delta_time);
	grass4.update(global_delta_time);
	
	if(keyboard.isKeyDown(GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(window, true);
	if(keyboard.isKeyPressed(GLFW_KEY_W)) global_camera.moveGlobalForward(global_delta_time);
	if(keyboard.isKeyPressed(GLFW_KEY_S)) global_camera.moveGlobalBackward(global_delta_time);
	if(keyboard.isKeyPressed(GLFW_KEY_A)) global_camera.moveLeft(global_delta_time);
	if(keyboard.isKeyPressed(GLFW_KEY_D)) global_camera.moveRight(global_delta_time);
	if(keyboard.isKeyPressed(GLFW_KEY_SPACE)) global_camera.moveGlobalUp(global_delta_time);
	if(keyboard.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) global_camera.moveGlobalDown(global_delta_time);
	if(keyboard.isKeyDown(GLFW_KEY_F)) flashlight.toggle();

	cube_map.update(global_delta_time);
	water.update(global_delta_time);
	flashlight.update(global_camera.position, global_camera.front);
	
	// draw
	auto render = [&]() {
	    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	    glm::mat4 projection = global_camera.getProjectionMatrix((float)Display::kWindowWidth,
								     (float)Display::kWindowHeight,
								     Display::kNearPlane, Display::kFarPlane);
	    glm::mat4 view = global_camera.getViewMatrix();

	    color_shader.bind();
	    color_shader.setFloat("shininess", 10.0f);
	    model.useDirectionalLight(global_camera.position, dir_light);
	    model.useSpotLight(global_camera.position, flashlight.spot_light);
	    model.usePointLight(global_camera.position, point_light);	    
	    model.draw(projection, view);

	    grass.draw(projection, view);
	    grass2.draw(projection, view);
	    grass3.draw(projection, view);
	    grass4.draw(projection, view);
	    
	    glm::mat4 view_no_translation = glm::mat4(glm::mat3(global_camera.getViewMatrix()));
	    cube_map.draw(projection, view_no_translation);
	};

	glEnable(GL_CLIP_DISTANCE0);

	// reflection
	water.bindReflectionFrameBuffer();
	float distance = 2 * (global_camera.position.y - water.position.y);
	global_camera.position.y -= distance;
	global_camera.setPitch(-global_camera.getPitch());
	color_shader.bind();
	color_shader.setVector4f("plane", down_clip_plane); // NOTE: Test edges with +1.0
	Shader::unbind();
	render();
	global_camera.position.y += distance;
	global_camera.setPitch(-global_camera.getPitch());

	// refraction
	water.bindRefractionFrameBuffer();
	color_shader.bind();
	color_shader.setVector4f("plane", up_clip_plane); // NOTE: test edges with +1.0 or -1.0
	Shader::unbind();
	render();
	
	color_shader.bind();
	color_shader.setVector4f("plane", high_clip_plane);
	Shader::unbind();
	
	water.unbindCurrentFrameBuffer();
	glDisable(GL_CLIP_DISTANCE0);

	// basic rendering
	render();
	glm::mat4 projection = global_camera.getProjectionMatrix((float)Display::kWindowWidth,
								 (float)Display::kWindowHeight,
								 Display::kNearPlane, Display::kFarPlane);
	glm::mat4 view = global_camera.getViewMatrix();
	water.draw(projection, view, global_camera.position);
	
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
    global_camera.processMouseScroll((float)y_offset);
}
