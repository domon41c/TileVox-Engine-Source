//Copyright by Avixo-tech(domon41c)
//Open Source voxel engine - TileVox

//Including all libs
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Data_Engine/Classes/Window.h"
#include "Data_Engine/Classes/Window.cpp"
#include "Data_Engine/Classes/Events.h"
#include "Data_Engine/Classes/Events.cpp"
#include "Data_Engine/Shaders/Shader.h"
#include "Data_Engine/Shaders/Shader.cpp"
#include "Data_Engine/Includers/Img_loader.h"
#include "Data_Engine/Includers/Img_loader.cpp"
#include "Data_Engine/Shaders/Render.h"
#include "Data_Engine/Shaders/Render.cpp"

//Set namespace
using namespace std;

//Window resolution
int window_width = 1280;
int window_height = 720;

float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

//Window
int main()
{       
        //Window title and initialize       
        Window::initialize(window_width, window_height, "TileVox-Engine");

        //Initialize from events
        Events::initialize();

        Shader* shader = load_shader("Data_Engine/Resources/Shader.glslv", "Data_Engine/Resources/Shader.glslf");

        if (shader == nullptr){
                cout << "Error loading shaders (Data_Engine/Resources/Shader.glslv), (Data_Engine/Resources/Shader.glslf)";
                Window::terminate();
                return 1;
        }

        Texture* texture = load_texture("file.png");
        if (texture == nullptr){
                cout << "loading png file error!";
                delete shader;
                Window::terminate();
                return 1;
        }

        //VAO, VBO Shaders
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glClearColor(0.6f,0.62f,0.65f,1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Engine while
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

                shader->use();
                texture->bind();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                glUseProgram(0);

                Window::swapBuffers();
        }

        delete shader;
        delete texture;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        Window::terminate();
        return 0;
}