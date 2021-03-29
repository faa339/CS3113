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
#include "Util.h"
#include "Entity.h"

#define PLATFORM_COUNT 17
#define ENEMY_COUNT 3
#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Entity* player;
    Entity* enemies;
    Entity* platforms;
    GLuint font;
};

GameState state;

void platformInit(GLuint Texture) {
    for (int i = 0; i < 10; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = Texture;
        state.platforms[i].isActive = true;
        state.platforms[i].position = glm::vec3(i - 4.5, -3.25, 0);
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, 0);
    }

    for (int i = 10; i < 14; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = Texture;
        state.platforms[i].isActive = true;
        state.platforms[i].position = glm::vec3((i + 1.5) - 10, -2.25, 0);
        state.platforms[i].Update(0, NULL,NULL, 0,NULL, 0);
    }

    for (int i = 14; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = Texture;
        state.platforms[i].isActive = true;
        state.platforms[i].position = glm::vec3((i - 5) - 12, 0.45, 0);
        state.platforms[i].Update(0, NULL,NULL, 0,NULL, 0);
    }
}


void enemyInit(GLuint Texture) {
    // I once heard from a professor that unwrapped loops
    // can sometimes work better on a low-level perspective 
    // I'm not too sure if that applies here but I figured it'd
    // be alright here 
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = CHASER;
    state.enemies[0].isActive = true;
    state.enemies[0].textureID = Texture;
    state.enemies[0].position = glm::vec3(2.0f, -1.20f, 0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[0].width = 0.7;
    state.enemies[0].height = 1;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].isActive = true;
    state.enemies[1].textureID = Texture;
    state.enemies[1].position = glm::vec3(-2.9, 2.0, 0);
    state.enemies[1].speed = 0.8f;
    state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[1].startPos = state.enemies[1].position;
    state.enemies[1].patrolRange = 2.0f;
    state.enemies[1].width = 0.7;
    state.enemies[1].height = 1;


    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = JUMPER;
    state.enemies[2].isActive = true;
    state.enemies[2].textureID = Texture;
    state.enemies[2].position = glm::vec3(3.6, -1.2, 0);
    state.enemies[2].speed = 1.0f;
    state.enemies[2].jumpPower = 5.0f;
    state.enemies[2].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[2].width = 0.7;
    state.enemies[2].height = 1;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Rise of the AI!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("SpingleFrames.png");
    state.player->position = glm::vec3(-4.5, -2.75, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f,0);
    state.player->speed = 3.5f;
    state.player->isActive = true;
    state.player->animCols = 4;
    state.player->animRows = 1;
    state.player->height = 0.8f;
    state.player->width = 0.4f;
    state.player->jumpPower = 8.0;
    state.player->right = true;

    GLuint GroundTexture = Util::LoadTexture("groundFloor.png");
    state.platforms = new Entity[PLATFORM_COUNT];
    platformInit(GroundTexture);

    GLuint EnemyTexture = Util::LoadTexture("horror.png");
    state.enemies = new Entity[ENEMY_COUNT];
    enemyInit(EnemyTexture);

    GLuint Font = Util::LoadTexture("font.png");
    state.font = Font;
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                break;

            case SDLK_RIGHT:
                break;

            case SDLK_SPACE:
                if (state.player->collidedBot && state.player->endBad==false) {
                    state.player->jump = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }


    if (state.player->endBad == false && state.player->endGood == false) {
        //No need for input if the game ended 
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            if (state.player->position.x > -4.6) {
                state.player->movement.x = -1.0f;
                state.player->left = true;
                state.player->right = false;
            }
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            if (state.player->position.x < 4.6) {
                state.player->movement.x = 1.0f;
                state.player->left = false;
                state.player->right = true;
            }

        }

        if (glm::length(state.player->movement) > 1.0f) {
            state.player->movement = glm::normalize(state.player->movement);
        }
    }
}

void Update() {
    if (state.player->endBad == false && state.player->endGood==false) {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;

        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }

        while (deltaTime >= FIXED_TIMESTEP) {

            for (int i = 0; i < ENEMY_COUNT; i++) {
                state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, NULL, 0);
            }
                
            state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);

            deltaTime -= FIXED_TIMESTEP;
        }

        accumulator = deltaTime;

        if (state.player->position.y > 3.25) {
            state.player->position.y = 3.25;
            state.player->velocity.y = 0;
        }
            
    }
    int deadCount = 0;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive == false)deadCount++;
    }
    if (deadCount == 3)
        state.player->endGood = true;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    
    for (int i = 0; i < PLATFORM_COUNT; i++)
        state.platforms[i].Render(&program);

    for (int i = 0; i < ENEMY_COUNT; i++) 
        state.enemies[i].Render(&program);
    
    state.player->Render(&program);
    
    if (state.player->endBad) {
        Util::DrawText(&program, state.font, "Spingle has Died", 0.5, .005, glm::vec3(-3.8,0,0));
    }
    if (state.player->endGood) {
        Util::DrawText(&program, state.font, "Spingle succeeds!", 0.5, .005, glm::vec3(-3.8, 0, 0));
    }
    SDL_GL_SwapWindow(displayWindow);

}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]){
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}