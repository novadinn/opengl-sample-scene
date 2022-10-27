#include "keyboard.h"

#include <GLFW/glfw3.h>

void Keyboard::addKeyBinding(int key) {
    keys_[key] = RELEASED;
}
    
void Keyboard::updateKeyStates(GLFWwindow* window) {
    for (std::map<int, KeyState>::iterator it = keys_.begin();
	 it != keys_.end();
	 ++it) {
	
	int state = glfwGetKey(window, it->first);
	if (state == GLFW_PRESS) {
	    switch(it->second) {
	    case RELEASED: {
		keys_[it->first] = DOWN;
	    } break;
	    case DOWN: {
		keys_[it->first] = PRESSED;
	    } break;
	    }
	} else if(state == GLFW_RELEASE) {
	    switch(it->second) {
	    case PRESSED: {
		keys_[it->first] = UP;
	    } break;
	    case UP: {
		keys_[it->first] = RELEASED;
	    } break;
	    }
	}
    }
}

bool Keyboard::isKeyUp(int key) {
    return keys_[key] == UP;
}

bool Keyboard::isKeyPressed(int key) {
    return keys_[key] == PRESSED;
}

bool Keyboard::isKeyDown(int key) {
    return keys_[key] == DOWN;
}

bool Keyboard::isKeyReleased(int key) {
    return keys_[key] == RELEASED;
}
