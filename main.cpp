//Copyright by Avixo-tech(domon41c)
//Open Source voxel engine - TileVox

//Including all libs
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Classes/Window.h"
#include "Classes/Window.cpp"
#include "Classes/Events.h"
#include "Classes/Events.cpp"

//Set namespace
using namespace std;

//Window resolution
int window_width = 1280;
int window_height = 720;

//Window
int main()
{       
        //Window title and initialize       
        Window::initialize(window_width, window_height, "TileVox-Engine");

        //Initialize from events
        Events::initialize();

        //Engine while
        glClearColor(0,0,0,1);
        while (!Window::isShouldClose()){
                Events::pullEvents();
                //if pressed key ESC window has closed
                if (Events::jpressed(GLFW_KEY_ESCAPE)){
                        Window::setShouldClose(true);
                }
                //if pressed mouse button color has set to red
                if (Events::jclicked(GLFW_MOUSE_BUTTON_1)){
                        glClearColor(1,0,0,1);
                }

                glClear(GL_COLOR_BUFFER_BIT);

                Window::swapBuffers();
        }

        Window::terminate();
        return 0;
}