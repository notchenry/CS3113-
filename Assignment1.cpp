//
//  Assignment1.cpp
//  Assignment1
//
//  Created by Henry Chen on 2/11/18.
//  Copyright © 2018 Mochi Chen. All rights reserved.
//

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filepath, GLint filtering);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Assignment1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glewInit();
    
    glViewport(0, 0, 1280, 720);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.5, 0.0, 0.6, 1);

    ShaderProgram program, program_tex;
    program.Load("vertex.glsl", "fragment.glsl");
    program_tex.Load("vertex_textured.glsl", "fragment_textured.glsl");
    program.SetColor(0.3, 0.3, 0.6, 1);
    program_tex.SetColor(0.6, 0.3, 0.3, 1);
    
    GLuint friedChicken = LoadTexture("./assets/friedchicken", GL_NEAREST);
    GLuint iceCream = LoadTexture("./assets/ice cream.jpg", GL_NEAREST);
    GLuint pizza = LoadTexture("./assets/pizza.jpg", GL_NEAREST);
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    float lastFrameTicks = 0.0f;
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        program_tex.SetModelMatrix(modelMatrix);
        program_tex.SetProjectionMatrix(projectionMatrix);
        program_tex.SetViewMatrix(viewMatrix);
        
        glUseProgram(program.programID);
        
        float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        modelMatrix.Translate(0, 1.0, 0);
        program.SetModelMatrix(modelMatrix);
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        modelMatrix.Identity();
        
        glUseProgram(program_tex.programID);
        
        glBindTexture(GL_TEXTURE_2D, squareStone);
        
        modelMatrix.Translate(0, -2.0, 0);
        program_tex.SetModelMatrix(modelMatrix);
        modelMatrix.Identity();
        
        glVertexAttribPointer(program_tex.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program_tex.positionAttribute);
        
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program_tex.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program_tex.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program_tex.positionAttribute);
        glDisableVertexAttribArray(program_tex.texCoordAttribute);
        
        
        glBindTexture(GL_TEXTURE_2D, circleStone);
        
        modelMatrix.Translate(2, 0, 0);
        program_tex.SetModelMatrix(modelMatrix);
        modelMatrix.Identity();
        
        glVertexAttribPointer(program_tex.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program_tex.positionAttribute);
        
        glVertexAttribPointer(program_tex.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program_tex.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program_tex.positionAttribute);
        glDisableVertexAttribArray(program_tex.texCoordAttribute);
        
        glBindTexture(GL_TEXTURE_2D, debrisGlass);
        
        program_tex.SetModelMatrix(modelMatrix);
        modelMatrix.Identity();
        
        glVertexAttribPointer(program_tex.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program_tex.positionAttribute);
        
        glVertexAttribPointer(program_tex.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program_tex.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program_tex.positionAttribute);
        glDisableVertexAttribArray(program_tex.texCoordAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}

GLuint LoadTexture(const char *filepath, GLint filtering) {
    int w,h,comp;
    unsigned char* image = stbi_load(filepath, &w, &h, &comp, STBI_rgb_alpha);
    if(image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    
    stbi_image_free(image);
    return retTexture;


