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
#include <vector>
#include "Entity.h"

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

#define PLATFORM_COUNT 29

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* successEnd;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool result = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTexture;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
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


void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position) {
    
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);

}

void platformInit(Entity* platforms) {

    for (int i = 0; i < 10; i++) {
        if (i == 4) {
            continue;
        }
        platforms[i].position.y = -3.25;
        platforms[i].position.x = i - 4.5;
        platforms[i].Update(0, NULL, 0);
    }
    for (int i = 10; i < 17; i++) {
        platforms[i].position.x = -4.5;
        platforms[i].position.y = (i - 2.25) - 10;
        platforms[i].Update(0, NULL, 0);
    }
    for (int i = 17; i < 24; i++) {
        platforms[i].position.x = 4.5;
        platforms[i].position.y = (i - 2.25) - 17;
        platforms[i].Update(0, NULL, 0);
    }
    for (int i = 24; i < PLATFORM_COUNT; i++) {
        platforms[i].position.y = 1;
        platforms[i].position.x = (i - 2) - 24;
        platforms[i].Update(0, NULL, 0);
    }
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

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontTexture = LoadTexture("font.png");

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->isActive = true;
    state.player->position = glm::vec3(0,3.2,0);
    state.player->acceleration = glm::vec3(0, -1.0f, 0);
    state.player->textureID = LoadTexture("ship.png");

    state.player->height = 0.4;
    state.player->width = 0.78;

    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTex = LoadTexture("dieTile.png");
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].textureID = platformTex;
        state.platforms[i].isActive = true;
        state.platforms[i].position = glm::vec3(0, 0, 0);
        state.platforms[i].entityType = BADPLATFORM;
    }
    state.platforms[4].isActive = false;
    platformInit(state.platforms);

    state.successEnd = new Entity();
    state.successEnd->isActive = true;
    state.successEnd->entityType = GOODPLATFORM;
    state.successEnd->textureID = LoadTexture("winTile.png");
    state.successEnd->position = glm::vec3(-0.5,-3.0f,0);
    state.successEnd->Update(0,NULL,0);
}


void ProcessInput() {
    //TODO: Make rotation on left/right, thrust on up

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (state.player->endBad == false && state.player->endGood == false) {
        if (keys[SDL_SCANCODE_LEFT]) {
            if (state.player->acceleration.x > -1.0f)
                state.player->acceleration.x -= .1f;
        }
        else {
            if (!keys[SDL_SCANCODE_RIGHT]) {
                if (state.player->acceleration.x < 0)
                    state.player->acceleration.x += .1f;
            }
        }

        if (keys[SDL_SCANCODE_RIGHT]) {
            if (state.player->acceleration.x < 1.0f)
                state.player->acceleration.x += .1f;
        }
        else {
            if (!keys[SDL_SCANCODE_LEFT]) {
                if (state.player->acceleration.x > 0)
                    state.player->acceleration.x -= .1f;
            }
        }

        if (keys[SDL_SCANCODE_UP]) {
            if (state.player->acceleration.y < 1.0f)
                state.player->acceleration.y += 0.1f;
        }
        else {
            if (state.player->acceleration.y > -1.0f)
                state.player->acceleration.y -= 0.1f;
        }
    }

}


void Update() {
    if (state.player->endBad == true || state.player->endGood == true)
        return;
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
        state.player->Update(FIXED_TIMESTEP, state.successEnd, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
        if (state.player->endBad || state.player->endGood)
            break;
    }

    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < PLATFORM_COUNT; i++)
        state.platforms[i].Render(&program);
    
    state.successEnd->Render(&program);
    state.player->Render(&program);

    if (state.player->endBad == true)
        DrawText(&program, fontTexture, "Mission Failed", 0.5f, -0.05f, glm::vec3(-2.9, 2.5, 0));
    if (state.player->endGood == true)
        DrawText(&program, fontTexture, "Mission Successful", 0.4f, -0.05f, glm::vec3(-2.9, 2.5, 0));

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