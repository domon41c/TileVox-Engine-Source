//Copyright by Avixo-tech(domon41c)
//Open Source voxel engine - TileVox

//Including all libs
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Data_Engine/Classes/Window.h"
#include "Data_Engine/Classes/Window.cpp"
#include "Data_Engine/Classes/Events.h"
#include "Data_Engine/Classes/Events.cpp"
#include "Data_Engine/Classes/Camera.h"
#include "Data_Engine/Classes/Camera.cpp"
#include "Data_Engine/Shaders/Shader.h"
#include "Data_Engine/Shaders/Shader.cpp"
#include "Data_Engine/Includers/Img_loader.h"
#include "Data_Engine/Includers/Img_loader.cpp"
#include "Data_Engine/Shaders/Render.h"
#include "Data_Engine/Shaders/Render.cpp"

using namespace std;

//Window resolution
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

//Sprites coordinates
float vertices[] = {
            // x    y     z     u     v
           -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
           -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
           -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            // x    y     z     u     v
};


int main() {
        //Res, Res, Title window
        Window::initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "TileVox-Engine: main scene");

        Events::initialize();

        //Load shaders
        Shader* shader = load_shader("Data_Engine/Resources/Shader.glslv", "Data_Engine/Resources/Shader.glslf");

        if (shader == nullptr){
                cout << "TileVox-Engine Error: shaders not loading -> terminate window";
                Window::terminate();
                return 1;
        }

        //Load sprite texture
        Texture* texture = load_texture("Data_Engine/Sprites/file.png");
        if (texture == nullptr){
                cout << "TileVox-Engine Error: textures not loading -> terminate window";
                delete shader;
                Window::terminate();
                return 1;
        }

        //Creating VAO, VBO shaders
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glClearColor(0.0f,0.80f,100.65f,5);

        
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Camera
        Camera* camera = new Camera(vec3(0,0,1), radians(90.0f));

        mat4 model(1.0f);
        model = translate(model, vec3(0.5f,0,0));

        //Float time
        float lastTime = glfwGetTime();
        float delta = 0.0f;

        //Camera float
        float camX = 0.0f;
        float camY = 0.0f;

        float speed = 5;
        float speed_sprint = 50;

        Events::toogleCursor();

        //main while of engine window
        while (!Window::isShouldClose()){

                //Time
                float currentTime = glfwGetTime();
                delta = currentTime - lastTime;
                lastTime = currentTime;

                //Key bindings and movement camera

                //if press key "1" - toogle cursor
                if (Events::jpressed(GLFW_KEY_1)){
                        Events::toogleCursor();
                }
                //if press key "ESC" - close window
                if (Events::jpressed(GLFW_KEY_ESCAPE)){
                        Window::setShouldClose(true);
                }
                //if press key "W" - move camera UP
                if (Events::pressed(GLFW_KEY_W)){
                        camera->position += camera->front * delta * speed;
                }
                //if press key "Q" - move camera UP faster
                if (Events::pressed(GLFW_KEY_Q)){
                        camera->position += camera->front * delta * speed_sprint;
                }
                //if press key "S" - move camera DOWN
                if (Events::pressed(GLFW_KEY_S)){
                        camera->position -= camera->front * delta * speed;
                }
                //if press key "D" - move camera RIGHT
                if (Events::pressed(GLFW_KEY_D)){
                        camera->position += camera->right * delta * speed;
                }
                //if press key "A" - move camera LEFT
                if (Events::pressed(GLFW_KEY_A)){
                        camera->position -= camera->right * delta * speed;
                }

                //Main Cursor|Mouse
                if (Events::_cursor_locked){
                        camY += -Events::deltaY / Window::height * 2;
                        camX += -Events::deltaX / Window::height * 2;

                        if (camY < -radians(89.0f)){
                                camY = -radians(89.0f);
                        }
                        if (camY > radians(89.0f)){
                                camY = radians(89.0f);
                        }

                        camera->rotation = mat4(1.0f);
                        camera->rotate(camY, camX, 0);
                }

                glClear(GL_COLOR_BUFFER_BIT);

                //loading shaders
                shader->use();
                shader->uniformMatrix("model", model);
                shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
                texture->bind();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                Window::swapBuffers();
                Events::pullEvents();
        }

        delete shader;
        delete texture;
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);

        Window::terminate();
        return 0;
}