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

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool playing = false; 
bool wallHit = false;


ShaderProgram program;
glm::mat4 viewMatrix, player1Matrix, player2Matrix, ballMatrix, projectionMatrix;


float lastTicks = 0.0f;
float player_speed = 3.0f;
float ball_speed = 2.0f;

glm::vec3 player1_position = glm::vec3(-4.75f, 0.0f, 0.0f);
glm::vec3 player1_movement = glm::vec3(0, 0, 0);

glm::vec3 player2_position = glm::vec3(4.75f, 0.0f, 0.0f);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);

glm::vec3 player_scale = glm::vec3(0.5f, 2.0f, 0.0f);

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    player1Matrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);

    player1Matrix = glm::translate(player1Matrix, player1_position);
    player1Matrix = glm::scale(player1Matrix, player_scale);

    player2Matrix = glm::translate(player2Matrix, player2_position);
    player2Matrix = glm::scale(player2Matrix, player_scale);

    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.5f,0.5f,0.0f));

    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.8f, 0.8f, 0.8f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ProcessInput() {
    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);

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
    // Let the controls and movement work until the ball hits the wall 
    if (wallHit == false) {
        
        // Startup the ball movement 
        if (playing==false) {
            if (keys[SDL_SCANCODE_SPACE]) {
                playing = true;
                ball_movement.x = 0.0f;
                ball_movement.y = 1.0f;
            }
        }
        
        if (keys[SDL_SCANCODE_W]) {
            player1_movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_S]) {
            player1_movement.y = -1.0f;
        }

        if (keys[SDL_SCANCODE_UP]) {
            player2_movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            player2_movement.y = -1.0f;
        }

    }
    
}

//Collision with player paddles 
bool leftCollision() {
    float xdist = fabs(ball_position.x - player1_position.x) - ((1.0f)/2.0f); //Both are 0.5f wide 
    float ydist = fabs(ball_position.y - player1_position.y) - ((2.5f)/2.0f); //Ball is 0.5f high
    return ((xdist < 0) && (ydist < 0));
}

bool rightCollision() {
    float xdist = fabs(ball_position.x - player2_position.x) - ((1.0f) / 2.0f); //Both are 0.5f wide 
    float ydist = fabs(ball_position.y - player2_position.y) - ((2.5f) / 2.0f); //Ball is 0.5f high
    return ((xdist < 0) && (ydist < 0));
}

//Collision with the roof/floor of the window
bool RFCollision() {
    float roofdist = fabs(ball_position.y - 7.5f) - ((7.5f+0.5f)/2.0f);
    
    return ((roofdist < 0));
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    player1_position += player1_movement * player_speed * deltaTime;
    player2_position += player2_movement * player_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;

    player1Matrix = glm::mat4(1.0f);
    player1Matrix = glm::translate(player1Matrix, player1_position);
    player1Matrix = glm::scale(player1Matrix, player_scale);

    player2Matrix = glm::mat4(1.0f);
    player2Matrix = glm::translate(player2Matrix, player2_position);
    player2Matrix = glm::scale(player2Matrix, player_scale);

    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.5f, 0.5f, 0.0f));

    if (leftCollision() || rightCollision() ) {
        ball_movement.x *= -1.0f;
    }
    if (RFCollision()) {
        ball_movement.y *= -1.0f;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetModelMatrix(player1Matrix);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(player2Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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