#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, princeMatrix, kingMatrix, earthMatrix, spaceMatrix, projectionMatrix;

GLuint princeTextureID;
GLuint earthTextureID;
GLuint kingTextureID;
GLuint spaceTextureID;

bool growing = true;
float prince_x = 0;
float prince_rotate = 0;
float earth_rotate = 0;
float king_scale = 2;
float lastTicks = 0;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image -- make sure path correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    //Set up all of our matrices for the scene
    viewMatrix = glm::mat4(1.0f);

    princeMatrix = glm::mat4(1.0f);
    princeMatrix = glm::translate(princeMatrix, glm::vec3(0.0f, 1.7f, 0.0f));

    kingMatrix = glm::mat4(2.0f);

    earthMatrix = glm::mat4(1.0f);
    earthMatrix = glm::scale(earthMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
    
    spaceMatrix = glm::mat4(1.0f);
    spaceMatrix = glm::scale(spaceMatrix, glm::vec3(10.0f, 10.0f, 1.0f));

    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    princeTextureID = LoadTexture("Sprites/KTPrince.png");
    earthTextureID = LoadTexture("Sprites/Earth.png");
    kingTextureID = LoadTexture("Sprites/KTKing.png");
    spaceTextureID = LoadTexture("Sprites/Space.jpg");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    //Modify our variables to get things translating and rotating properly
    prince_x = -1.0f * deltaTime;
    prince_rotate = 35.2 * deltaTime;
    earth_rotate = -15 * deltaTime;

    //For the king, I chose to redraw and replace him every update instead of placing once
    //and scaling him up and down because I wasn't too sure how to get his current size or 
    //individual numbers from his matrix -- seems to work fine for now

    if (king_scale > 2.5f || king_scale <  1.6f) {
        growing = !growing;
    }
 
    king_scale += growing ? 0.01f : -0.01f;

    princeMatrix = glm::rotate(princeMatrix, glm::radians(prince_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    princeMatrix = glm::translate(princeMatrix, glm::vec3(prince_x,0.0f,0.0f));

    earthMatrix = glm::rotate(earthMatrix, glm::radians(earth_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    
    kingMatrix = glm::mat4(2.0f);
    kingMatrix = glm::translate(kingMatrix, glm::vec3(2.9f, 2.1f, 0.0f));
    kingMatrix = glm::scale(kingMatrix, glm::vec3(king_scale, king_scale, 0.0f));

}

//Just methods to do the drawing for each sprite, as per the video
void DrawPrince() {
    program.SetModelMatrix(princeMatrix);
    glBindTexture(GL_TEXTURE_2D, princeTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawEarth() {
    program.SetModelMatrix(earthMatrix);
    glBindTexture(GL_TEXTURE_2D, earthTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawKing() {
    program.SetModelMatrix(kingMatrix);
    glBindTexture(GL_TEXTURE_2D, kingTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawSpace() {
    //I also wasn't sure if there was a way to render space only once 
    program.SetModelMatrix(spaceMatrix);
    glBindTexture(GL_TEXTURE_2D, spaceTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    DrawSpace();

    DrawPrince();

    DrawEarth();

    DrawKing();

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}