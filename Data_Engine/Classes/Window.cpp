//Window events

#include <iostream>
#include "Window.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>

using namespace std;

GLFWwindow* Window::window;

int Window::initialize(int window_width, int window_height, const char* title){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(window_width, window_height, title, nullptr, nullptr);

    if (window == nullptr){
    	cout << "TileVox-Engine Error: Failed starting engine window.";
    	glfwTerminate();
    	return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
    	cout << "TileVox-Engine Error: OpenGL not loaded, please reinstall OpenGL";
    	return -1;
    }

    glViewport(0,0, window_width, window_height);
    return 0;
}

bool Window::isShouldClose(){
	return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool flag){
    glfwSetWindowShouldClose(window, flag);
}

void Window::swapBuffers(){
	glfwSwapBuffers(window);
}

void Window::terminate(){

}