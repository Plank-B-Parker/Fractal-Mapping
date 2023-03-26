#include "Input/Mouse.h"
#include "Math/Complex.h"
#include <iostream>

complex mouse::pos;
float mouse::scroll = 1;
bool mouse::clicked;
float mouse::clickDecay;

extern int WINDOW_HEIGHT, WINDOW_WIDTH;    //global in main

void mouse::mouse_callback(GLFWwindow* window, double xpos, double ypos){

    float centreX = WINDOW_WIDTH/2.0f;
    float centreY = WINDOW_HEIGHT/2.0f;

    float diffX = xpos - centreX;
    float diffY = ypos - centreY;

    pos = complex(diffX/centreY, -diffY/centreY);

    scroll = 0;

}

void mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        clicked = GLFW_PRESS == action;
        clickDecay = 1;
    }
    
}

void mouse::mouse_scroll_callback(GLFWwindow* window, double scrollX, double scrollY){
    scroll = scrollY;
    if(-0.1f <= scroll && scroll <= 0.1f)
        scroll = 0.0f;
}

void mouse::update(float dt){
    if(!clicked)
        clickDecay *= (1 - 0.3 * dt);
}