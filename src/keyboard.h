#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <map>

struct Keyboard {
    Keyboard() {}

    void addKeyBinding(int key);
    
    void updateKeyStates(GLFWwindow* window);

    bool isKeyUp(int key);
    bool isKeyPressed(int key);
    bool isKeyDown(int key);
    bool isKeyReleased(int key);
private:
    enum KeyState {
	UP,
	PRESSED,
	DOWN,
	RELEASED
    };
    
    std::map<int, KeyState> keys_;
};

#endif
