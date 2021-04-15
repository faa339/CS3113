#include "Level2.h"
#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 16
#define ENEMY_COUNT 2

unsigned int level2_data[] =
{
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level2::Initialize() {
	state.nextScene = -1;

	GLuint mapTexID = Util::LoadTexture("tileset.png");

	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level2_data, mapTexID, 1.0f, 4, 1);
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->position = glm::vec3(5, -12, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -9.81f, 0);
	state.player->speed = 3.5f;
	state.player->textureID = Util::LoadTexture("SpingleFrames.png");
	state.player->right = true;
	state.player->animIndex = 0;
	state.player->animCols = 4;
	state.player->animRows = 1;
	state.player->height = 0.8;
	state.player->width = 0.8;
	state.player->jumpPower = 8.0;
	state.player->gotHit = false;
	state.player->endGood = false;

	GLuint enemyTex = Util::LoadTexture("horror.png");
	state.enemies = new Entity[ENEMY_COUNT];
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].aiType = JUMPER;
	state.enemies[0].isActive = true;
	state.enemies[0].textureID = enemyTex;
	state.enemies[0].position = glm::vec3(3, -8, 0);
	state.enemies[0].speed = 1.0f;
	state.enemies[0].jumpPower = 9.0f;
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].width = 0.7;
	state.enemies[0].height = 1;

	state.enemies[1].entityType = ENEMY;
	state.enemies[1].aiType = CHASER;
	state.enemies[1].aiState = IDLE;
	state.enemies[1].isActive = true;
	state.enemies[1].textureID = enemyTex;
	state.enemies[1].position = glm::vec3(1, -12.5, 0);
	state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
	state.enemies[1].speed = 1.0f;
	state.enemies[1].width = 0.7;
	state.enemies[1].height = 1;

};



void Level2::Update(float deltaTime) {
	for (int i = 0; i < ENEMY_COUNT; i++)
		state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player->gotHit) {
		Reset();
	}
	if (state.player->position.x > 8.8 && state.player->position.y > -2.5)
		state.nextScene = 3;
};



void Level2::Render(ShaderProgram* program) {
	state.map->Render(program);

	for (int i = 0; i < ENEMY_COUNT; i++)
		state.enemies[i].Render(program);

	state.player->Render(program);

};

void Level2::Reset() {
	//Reset player & enemies on enemy hit
	state.player->position = glm::vec3(4, -12.5, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -9.81, 0);

	state.enemies[0].position = glm::vec3(3, -8, 0);
	state.enemies[0].movement = glm::vec3(0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
	state.enemies[0].isActive = true;

	state.enemies[1].position = glm::vec3(1, -12, 0);
	state.enemies[1].movement = glm::vec3(0);
	state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
	state.enemies[1].aiState = IDLE;
	state.enemies[1].isActive = true;
}