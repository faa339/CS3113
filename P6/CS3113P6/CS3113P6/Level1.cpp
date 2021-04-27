#include "Level1.h"
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 20
#define ENEMY_COUNT 0

unsigned int level1_data[] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void Level1::Initialize() {
	state.nextScene = -1;

	GLuint mapTexID = Util::LoadTexture("mapTiles.png");

	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTexID, 1.0f, 8, 1);
	
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->textureID = Util::LoadTexture("test.png");
	state.player->position = glm::vec3(0);
	state.player->speed = 3.0f;
	state.player->maxAmmo = 12;
	state.player->Bullets = new Entity[state.player->maxAmmo];
	GLuint bullet = Util::LoadTexture("bullet.png");
	GLuint enemy = Util::LoadTexture("enemy.png");;
	for (int i = 0; i < state.player->maxAmmo; i++) {
		state.player->Bullets[i].entityType = BULLET;
		state.player->Bullets[i].speed = 9.5f;
		state.player->Bullets[i].textureID = bullet;
		state.player->Bullets[i].isActive = false;
	}
	state.player->isActive = true;

};



void Level1::Update(float deltaTime) {
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.player->Bullets, state.player->maxAmmo, state.map);
		if (state.enemies[i].Bullets != NULL) {//Good way to stop memory issues from early -- they don't all have guns!
			for (int j = 0; j < state.enemies[i].maxAmmo; j++) {
				state.enemies[i].Bullets[j].Update(deltaTime, state.player, NULL, 0, state.map);
			}
		}
	}
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].Update(deltaTime, state.player, NULL, 0, NULL);
};



void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	//for (int i = 0; i < ENEMY_COUNT; i++)
	//	state.enemies[i].Render(program);
	state.player->Render(program);
	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].Render(program);
};

void Level1::Reset() {
	//Reset player & enemies on enemy hit
	state.player->position = glm::vec3(4, -12.5, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -9.81, 0);

	state.enemies[0].aiState = IDLE;
	state.enemies[0].position = glm::vec3(8, -12.5, 0);
	state.enemies[0].movement = glm::vec3(0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
	state.enemies[0].isActive = true;

	state.enemies[1].position = glm::vec3(2.3, -4, 0);
	state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[1].isActive = true;
}