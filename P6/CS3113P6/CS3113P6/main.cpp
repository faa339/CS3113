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
#include "Startup.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#define SCENE_COUNT 4
SDL_Window* displayWindow;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 viewMatrixUI, projectionMatrixUI;
glm::vec3 currentMousePos;

int windowWidth = 640;
int windowHeight = 480;

//Mouse position floats -- for later use!
float worldX;
float worldY;

bool gameIsRunning = true;
bool paused;
bool peek;

GLuint crosshair;
GLuint font;

Scene* currentScene;
Scene* sceneList[SCENE_COUNT];

Mix_Music* music;

float gameTimer;

void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Revanchist", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
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

	viewMatrixUI = glm::mat4(1.0f);
	projectionMatrixUI = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);


	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	font = Util::LoadTexture("font.png", false);
	crosshair = Util::LoadTexture("crosshair.png", false);
	currentMousePos = glm::vec3(0);
	sceneList[0] = new Startup();
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	music = Mix_LoadMUS("HarmfulOrFatalKevinMacleod.mp3");
	Mix_PlayMusic(music, -1);
	Mix_Volume(-1, MIX_MAX_VOLUME / 6);


	SDL_SetRelativeMouseMode(SDL_TRUE);
	paused = false;
	SwitchToScene(sceneList[0]);
	gameTimer = 300.0f;
}

void ProcessInput() {
	SDL_Event event;
	int mouseX, mouseY;
	if(currentScene->state.player != NULL)
		currentScene->state.player->movement = glm::vec3(0);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			return;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: // Some pause functionality!
				if (currentScene != sceneList[0]) {
					if (paused == true) {
						SDL_SetRelativeMouseMode(SDL_TRUE);
						paused = false;
					}
					else {
						SDL_SetRelativeMouseMode(SDL_FALSE);
						paused = true;
					}
				}
				break;
			case SDLK_RETURN:
				if (currentScene == sceneList[0])
					currentScene->state.nextScene = 1;
			}			
		case SDL_MOUSEBUTTONDOWN:
			if (currentScene->state.player != NULL && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT 
				&& paused==false)
				currentScene->state.player->Shoot();
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	if (paused == false && currentScene->state.player != NULL) {
		if (keys[SDL_SCANCODE_A]) {
			currentScene->state.player->movement.x = -1.0f;
		}
		else if (keys[SDL_SCANCODE_D]) {
			currentScene->state.player->movement.x = 1.0f;
		}

		if (keys[SDL_SCANCODE_W] && currentScene->state.player->position.y<0) {
			currentScene->state.player->movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_S] && currentScene->state.player->position.y>-30.0f) {
			currentScene->state.player->movement.y = -1.0f;
		}

		if (keys[SDL_SCANCODE_LSHIFT]) {
			peek = true;
		}
		else
			peek = false;

		if (keys[SDL_SCANCODE_R])
			currentScene->state.player->reloading = true;

		if (glm::length(currentScene->state.player->movement) > 1.0f)
			currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);

		SDL_GetMouseState(&mouseX, &mouseY);

		float currWorldX = 10.0f; 
		float currWorldy = 7.5f;
		worldX = (((float)mouseX / windowWidth) * currWorldX) - (currWorldX / 2.0f);
		worldY = (((windowHeight - (float)mouseY) / windowHeight) * currWorldy) - (currWorldy/ 2.0f);
		currentMousePos.x = worldX + currentScene->state.player->position.x;
		currentMousePos.y = worldY + currentScene->state.player->position.y;

		float dX = currentMousePos.x - currentScene->state.player->position.x;
		float dY = currentMousePos.y - currentScene->state.player->position.y;
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

	if (paused == false && gameIsRunning==true) {
		while (deltaTime >= FIXED_TIMESTEP) {
			currentScene->Update(FIXED_TIMESTEP);
			deltaTime -= FIXED_TIMESTEP;
		}
		accumulator = deltaTime;
	
		if (currentScene->state.player != NULL) {
			viewMatrix = glm::mat4(1.0f);
			float viewYpos;
			if (peek == false) {
				viewYpos = (currentScene->state.player->position.y > -25.0f) ?
					((currentScene->state.player->position.y < -3.24f) ? -currentScene->state.player->position.y : 3.24f) : 25.0f;
				viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, viewYpos, 0));
			}
			else {
				viewYpos = (currentScene->state.player->position.y > -25.0f) ?
					((currentScene->state.player->position.y < -3.24f) ? -currentMousePos.y-0.7f : 3.24f) : 25.0f;
				viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentMousePos.x, viewYpos, 0));
			}
		}
	}
	if (currentScene != sceneList[0] && paused==false && gameIsRunning)
		gameTimer -= FIXED_TIMESTEP;
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	program.SetViewMatrix(viewMatrix);
	program.SetProjectionMatrix(projectionMatrix);
	currentScene->Render(&program);

	if (paused == false && currentScene != sceneList[0]) {
		Util::DrawIcon(&program, crosshair, glm::vec3(currentMousePos.x, currentMousePos.y, 0));
	}
	program.SetProjectionMatrix(projectionMatrixUI);
	program.SetViewMatrix(viewMatrixUI);
	if (currentScene == sceneList[0]) {
		Util::DrawText(&program, font, "REVANCHIST", 0.7, 0.02, glm::vec3(-3.2, 2, 0));
		Util::DrawText(&program, font, "Your spaceship has been taken", 0.2, 0.01, glm::vec3(-3.0, 1, 0));
		Util::DrawText(&program, font, "by interdimensional cyborgs.", 0.2, 0.01, glm::vec3(-2.75, 0.6, 0));
		Util::DrawText(&program, font, "Stop them! Before it's too late!", 0.2, 0.01, glm::vec3(-3.2, 0, 0));
		Util::DrawText(&program, font, "Press [ENTER] to begin!", 0.2, 0.01, glm::vec3(-2.25, -1, 0));
	}
	if (currentScene != sceneList[0] && currentScene->state.player != NULL) {
		Util::DrawText(&program, font, "Objective:", 0.2, 0.01, glm::vec3(-4.75,3.2,0));
		Util::DrawText(&program, font, "Save the ship!", 0.2, 0.01, glm::vec3(-4.75, 3.0, 0));
		std::string timer = std::to_string(int(gameTimer)) + " seconds left!";
		Util::DrawText(&program, font, timer, 0.2, 0.01, glm::vec3(1.25, 3.2, 0));
		int bullets = 12 - currentScene->state.player->currentBullet;
		if (currentScene->state.player->reloading==false) {
			std::string remainingBullets = std::to_string(bullets) + "/12";
			Util::DrawText(&program, font, remainingBullets, 0.2, 0.01, glm::vec3(3.75, -3.2, 0));
		}
		else {
			Util::DrawText(&program, font, "RELOADING", 0.2, 0.01, glm::vec3(3.15, -3.2, 0));
		}
		if (currentScene->state.player->endGood == true) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			paused = true; 
			Util::DrawText(&program, font, "The ship is saved!", 0.3, 0.01, glm::vec3(-2.75, 1, 0));
			Util::DrawText(&program, font, "Fly off, REVANCHIST!", 0.3, 0.01, glm::vec3(-3.0, 0.5, 0));
		}
		if (gameTimer <= 0) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			paused = true; 
			Util::DrawText(&program, font, "You were too late.", 0.3, 0.01, glm::vec3(-2.2, 1, 0));
			Util::DrawText(&program, font, "The ship was crashed into Earth.", 0.2, 0.01, glm::vec3(-3.0, 0, 0));
		}
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
		if (currentScene->state.nextScene != -1) { 
			int nextScene = currentScene->state.nextScene;
			SwitchToScene(sceneList[nextScene]); 
		}
		Render();
	}
	Shutdown();
	return 0;
}