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
#include "Map.h"
#include "Scene.h"
#include "Entity.h"
#include "Startup.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#define SCENE_COUNT 4
#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;


Scene* currentScene;
Scene* sceneList[SCENE_COUNT];


SDL_Window* displayWindow;
bool gameIsRunning = true;
ShaderProgram program;

glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
GLuint gameFont;
Mix_Music* music;
Mix_Chunk* bounce;

int LivesCount = 3;

void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Spingle's Greatest Adventure!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
	gameFont = Util::LoadTexture("font.png");

	sceneList[0] = new Startup();
	sceneList[0]->state.font = gameFont;
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("acidTrumpetKevinMacleod.mp3");
	bounce = Mix_LoadWAV("jalastramJump.wav");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_Volume(-1, MIX_MAX_VOLUME / 6);

	Mix_PlayMusic(music, -1);

	
	SwitchToScene(sceneList[0]);
}

void ProcessInput() {
	SDL_Event event;
	if (currentScene->state.player != NULL)
		currentScene->state.player->movement = glm::vec3(0);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_RETURN:
				if (currentScene == sceneList[0])
					currentScene->state.nextScene = 1;
				break;
			case SDLK_SPACE:
				if (currentScene->state.player != NULL && currentScene->state.player->collidedBot 
					&& (currentScene->state.player->endGood==false && LivesCount>0)) {
					currentScene->state.player->jump = true;
					Mix_PlayChannel(-1, bounce, 0);
				}
				break;
			}
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (currentScene->state.player != NULL && (LivesCount!=0) && currentScene->state.player->endGood==false){
		if (keys[SDL_SCANCODE_LEFT]) {
			currentScene->state.player->movement.x = -1.0f;
			currentScene->state.player->left = true;
			currentScene->state.player->right = false;
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			currentScene->state.player->movement.x = 1.0f;
			currentScene->state.player->left = false;
			currentScene->state.player->right = true;
		}

		if (glm::length(currentScene->state.player->movement) > 1.0f) {
			currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
		}

	}
	

}

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}
	while (deltaTime >= FIXED_TIMESTEP && LivesCount > 0) {
		currentScene->Update(FIXED_TIMESTEP);
		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
	if (currentScene->state.player != NULL) {
		viewMatrix = glm::mat4(1.0f);
		//Camera goes up and down by a set amount per stage -- its a little limiting but 
		//a little less tedious than doing a length per scene
		if (currentScene->state.player->position.y > -11.5 && currentScene->state.player->position.y < -3.75)
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
		else {
			if (currentScene->state.player->position.y <= -11.5) viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 11.5, 0));
			if (currentScene->state.player->position.y >= -3.75) viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
		}
		if(currentScene->state.player->gotHit && LivesCount!=0){
			LivesCount -= 1;
			currentScene->state.player->gotHit = false;
		}
	}
	
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	program.SetViewMatrix(viewMatrix);
	currentScene->Render(&program);
	if (currentScene != sceneList[0] && LivesCount != 0) {
		glm::vec3 textPos = glm::vec3(currentScene->state.player->position.x - 0.4, 
			currentScene->state.player->position.y + 0.7, 0);
		Util::DrawText(&program, gameFont, std::to_string(LivesCount), 0.2, 0.02, textPos);
	}
	if (LivesCount == 0) {
		glm::vec3 textPos = glm::vec3(currentScene->state.player->position.x - 2.5,
			currentScene->state.player->position.y + 0.5, 0);
		Util::DrawText(&program, gameFont, "Game over!", 0.5, 0.1, textPos);
	}
	if (currentScene->state.player!=NULL && currentScene->state.player->endGood) {
		glm::vec3 textPos = glm::vec3(currentScene->state.player->position.x - 5.75,
			currentScene->state.player->position.y - 2, 0);
		Util::DrawText(&program, gameFont, "You win!", 0.5, 0.1, textPos);
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
		if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
		Render();
	}
	Shutdown();
	return 0;
}