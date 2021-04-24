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
#include "SDL_mixer.h"

#include "Util.h"
#include "Entity.h"



SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Entity* player;

bool gameIsRunning = true;

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Endrun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

	player = new Entity();
	player->entityType = PLAYER;
	player->textureID = Util::LoadTexture("test.png");
	player->position = glm::vec3(0);
	player->speed = 1.0f;
	//SDL_SetRelativeMouseMode(SDL_TRUE); For later!
}

void ProcessInput() {
	SDL_Event event;
	player->movement = glm::vec3(0);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				//Dash idk
				break;
			}			
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	/*
	if (keys[SDL_SCANCODE_A]) {
		player->movement.x = -1.0f;
	}
	else if (keys[SDL_SCANCODE_D]) {
		player->movement.x = 1.0f;
	}
	
	if (keys[SDL_SCANCODE_W]) {
		player->movement.y = 1.0f;
	}
	else if (keys[SDL_SCANCODE_S]) {
		player->movement.y = -1.0f;
	}
	
	if (glm::length(player->movement) > 1.0f)
		player->movement = glm::normalize(player->movement);
		*/
	//Need to fix
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	float dX = player->position.x - mouseX;
	float dY = player->position.y - mouseY;
	player->lookRotation = glm::atan(dY, dX);

}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}
	while (deltaTime >= FIXED_TIMESTEP) {
		player->Update(FIXED_TIMESTEP, player, NULL, 0, NULL);
		deltaTime -= FIXED_TIMESTEP;
	}
	accumulator = deltaTime;


}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	player->Render(&program);
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