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
#include "Scene.h"
#include "Level1.h"

#define ENEMY_COUNT 0
#define SCENE_COUNT 1
SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

int windowWidth = 640;
int windowHeight = 480;

//Mouse position floats -- for later use!
float worldX;
float worldY;

bool gameIsRunning = true;
bool paused;

GLuint crosshair;

Scene* currentScene;
Scene* sceneList[SCENE_COUNT];

void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Endrun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, windowWidth, windowHeight);
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
	
	crosshair = Util::LoadTexture("crosshair.png");

	sceneList[0] = new Level1();
	/*
	enemies = new Entity[ENEMY_COUNT];
	enemies[0].entityType = ENEMY;
	enemies[0].aiType = SHOOTER;
	enemies[0].aiState = IDLE;
	enemies[0].textureID = enemy;
	enemies[0].position = glm::vec3(1, 0, 0);
	enemies[0].speed = 3.5f;
	enemies[0].maxAmmo = 3;
	enemies[0].Bullets = new Entity[enemies[0].maxAmmo];
	for (int i = 0; i < enemies[0].maxAmmo; i++) {
		enemies[0].Bullets[i].entityType = BULLET;
		enemies[0].Bullets[i].speed = 9.5f;
		enemies[0].Bullets[i].textureID = bullet;
		enemies[0].Bullets[i].isActive = false;
	}
	enemies[0].shootSpacing = 0.35f;
	enemies[0].isActive = true;

	enemies[1].entityType = ENEMY;
	enemies[1].aiType = DASHER;
	enemies[1].aiState = IDLE;
	enemies[1].textureID = enemy;
	enemies[1].position = glm::vec3(2, 0, 0);
	enemies[1].speed = 3.5f;
	enemies[1].waitTime = 0.8f;
	enemies[1].isActive = true;*/
	//Keep the mouse in the screen -- handy for later!
	SDL_SetRelativeMouseMode(SDL_TRUE);
	paused = false;
	SwitchToScene(sceneList[0]);
}

void ProcessInput() {
	SDL_Event event;
	int mouseX, mouseY;

	currentScene->state.player->movement = glm::vec3(0);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: // Some pause functionality!
				if (paused == true) {
					SDL_SetRelativeMouseMode(SDL_TRUE);
					paused = false;
				}
				else {
					SDL_SetRelativeMouseMode(SDL_FALSE);
					paused = true;
				}
				break;
			}			
		case SDL_MOUSEBUTTONDOWN:
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && paused==false)
				currentScene->state.player->Shoot();
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	if (paused == false) {
		if (keys[SDL_SCANCODE_A]) {
			currentScene->state.player->movement.x = -1.0f;
		}
		else if (keys[SDL_SCANCODE_D]) {
			currentScene->state.player->movement.x = 1.0f;
		}

		if (keys[SDL_SCANCODE_W]) {
			currentScene->state.player->movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_S]) {
			currentScene->state.player->movement.y = -1.0f;
		}

		if (glm::length(currentScene->state.player->movement) > 1.0f)
			currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);

		SDL_GetMouseState(&mouseX, &mouseY);

		//SDL_Log("Mouse positions %d %d", mouseX, mouseY);
		//float variableWorldWidth = windowWidth; //+ currentScene->state.player->position.x;
		//float variableWorldHeight = windowHeight; //+currentScene->state.player->position.y;
		float currWorldX = 10.0f; //+currentScene->state.player->position.x;
		float currWorldy = 7.5f;// +currentScene->state.player->position.y;
		worldX = (((float)mouseX / windowWidth) * currWorldX) - (currWorldX / 2.0f);
		worldY = (((windowHeight - (float)mouseY) / windowHeight) * currWorldy) - (currWorldy/ 2.0f);
		SDL_Log("World positions %f %f\n Screen positions %d %d", worldX, worldY, mouseX, mouseY);


		float dX = worldX - currentScene->state.player->position.x;
		float dY = worldY - currentScene->state.player->position.y;
		currentScene->state.player->dirVec = glm::normalize(glm::vec3(dX, dY, 0));
		currentScene->state.player->lookRotation = glm::atan(currentScene->state.player->dirVec.y, 
			                                                 currentScene->state.player->dirVec.x);
	}
	
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

	if (paused == false) {
		while (deltaTime >= FIXED_TIMESTEP) {
			currentScene->Update(FIXED_TIMESTEP);
			deltaTime -= FIXED_TIMESTEP;
		}
		accumulator = deltaTime;
	
		if (currentScene->state.player != NULL) {
			viewMatrix = glm::mat4(1.0f);
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 
				                                              -currentScene->state.player->position.y, 0));
		}
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	program.SetViewMatrix(viewMatrix);
	currentScene->Render(&program);
	if(paused == false)
		Util::DrawIcon(&program, crosshair, glm::vec3(worldX, worldY, 0));
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