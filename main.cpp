//Copyright by Avixo-tech(domon41c)
//Open Source voxel engine - TileVox

//Including all libs
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
#include "Data_Engine/Shaders/Meshes.h"
#include "Data_Engine/Shaders/Meshes.cpp"
#include "Data_Engine/Vox/vox.h"
#include "Data_Engine/Vox/vox.cpp"
#include "Data_Engine/Vox/Chunks.h"
#include "Data_Engine/Vox/Chunks.cpp"
#include "Data_Engine/Shaders/Vox_Gen.h"
#include "Data_Engine/Shaders/Vox_Gen.cpp"
#include "Data_Engine/Vox/Chunks_.h"
#include "Data_Engine/Vox/Chunks_.cpp"

using namespace std;

//Window resolution
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

//Sprites coordinates
float vertices[] = {
            // x    y   
           -0.01f,-0.01f,
            0.01f, 0.01f,

           -0.01f, 0.01f,
            0.01f,-0.01f,
};

int attrs[] = {
        2, 0
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

        Shader* crosshairShader = load_shader("Data_Engine/Resources/CrossHair.glslv", "Data_Engine/Resources/CrossHair.glslf");

        if (crosshairShader == nullptr){
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

        Chunks* chunks = new Chunks(8,1,8);
        Mesh** meshes = new Mesh*[chunks->volume];
        for (size_t i = 0; i < chunks->volume; i++)
                meshes[i] = nullptr;
        VoxelRenderer renderer(1024*1024*8);
        

        glClearColor(0.0f,0.80f,100.65f,5);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Mesh* crosshair = new Mesh(vertices, 4, attrs);
        //Camera
        Camera* camera = new Camera(vec3(10,15,10), radians(90.0f));


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

                {
                        vec3 end;
                        vec3 norm;
                        vec3 iend;
                        voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);

                        if (vox != nullptr){
                                if (Events::jclicked(GLFW_MOUSE_BUTTON_1)){
                                        chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
                                }
                                if (Events::jclicked(GLFW_MOUSE_BUTTON_2)){
                                        chunks->set((int)(iend.x)+(int)(norm.x), (int)(iend.y)+(int)(norm.y), (int)(iend.z)+(int)(norm.z), 4);
                                }
                        }
                }

                Chunk* closes[27];
                for (size_t i = 0; i < chunks->volume; i++){
                        Chunk* chunk = chunks->chunks[i];

                        if (!chunk->modified)
                                continue;
                        chunk->modified = false;

                        if (meshes[i] != nullptr)
                                delete meshes[i];

                        for (int i = 0; i < 27; i++)
                                closes[i] = nullptr;
                        for (size_t j = 0; j < chunks->volume; j++){
                                Chunk* other = chunks->chunks[j];
                                int ox = other->x - chunk->x;
                                int oy = other->y - chunk->y;
                                int oz = other->z - chunk->z;

                                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                                        continue;
                        
                                ox += 1;
                                oy += 1;
                                oz += 1;
                                closes[(oy * 3 + oz) * 3 + ox] = other;
                }

                Mesh* mesh = renderer.render(chunk, (const Chunk**)closes);
                meshes[i] = mesh;
        }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //loading shaders
                shader->use();
                shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
                texture->bind();
                mat4 model(1.0f);
                for (size_t i = 0; i < chunks->volume; i++){
                        Chunk* chunk = chunks->chunks[i];
                        Mesh* mesh = meshes[i];
                        model = glm::translate(mat4(1.0f), vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
                        shader->uniformMatrix("model", model);
                        mesh->draw(GL_TRIANGLES);
                }
                crosshairShader->use();
                crosshair->draw(GL_LINES);
                Window::swapBuffers();
                Events::pullEvents();
        }

        delete shader;
        delete texture;
        delete chunks;
        delete crosshair;
        delete crosshairShader;

        Window::terminate();
        return 0;
}