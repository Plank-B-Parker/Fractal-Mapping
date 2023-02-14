#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>

struct complex;

struct mouse{
    static complex pos;
    static float scroll;
    static bool clicked;
    static float clickDecay;

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow* window, double scrollX, double scrollY);
    static void update(float dt);

    private:
        mouse(){}
    
};