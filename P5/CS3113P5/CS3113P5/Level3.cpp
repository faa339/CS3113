#include "Level3.h"
#define LEVEL3_WIDTH 11
#define LEVEL3_HEIGHT 16
#define ENEMY_COUNT 2


unsigned int level3_data[] =
{
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
	3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level3::Initialize() {
	state.nextScene = -1;

	GLuint mapTexID = Util::LoadTexture("tileset.png");

	state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTexID, 1.0f, 4, 1);
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
	state.enemies[0].aiType = CHASER;
	state.enemies[0].aiState = WALKING;
	state.enemies[0].isActive = true;
	state.enemies[0].textureID = enemyTex;
	state.enemies[0].position = glm::vec3(1.0, -13, 0);
	state.enemies[0].speed = 1.5f;
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].width = 0.7;
	state.enemies[0].height = 1;

	state.enemies[1].entityType = ENEMY;
	state.enemies[1].aiType = CHASER;
	state.enemies[1].aiState = WALKING;
	state.enemies[1].isActive = true;
	state.enemies[1].textureID = enemyTex;
	state.enemies[1].position = glm::vec3(9.0, -13, 0);
	state.enemies[1].speed = 1.5f;
	state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[1].width = 0.7;
	state.enemies[1].height = 1;

};

void Level3::Update(float deltaTime) {
	for (int i = 0; i < ENEMY_COUNT; i++)
		state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player->gotHit) {
		Reset();
	}
	if (state.player->position.x > 8.8 && state.player->position.y > -2.5) {
		state.player->endGood = true;
	}
};

void Level3::Render(ShaderProgram* program) {
	state.map->Render(program);
	for (int i = 0; i < ENEMY_COUNT; i++)
		state.enemies[i].Render(program);
	state.player->Render(program);
};

void Level3::Reset() {
	//Reset player & enemies on enemy hit
	state.player->position = glm::vec3(5, -12, 0);
	state.player->movement = glm::vec3(0);
	state.player->acceleration = glm::vec3(0, -9.81, 0);

	state.enemies[0].position = glm::vec3(1.0, -13, 0);
	state.enemies[0].movement = glm::vec3(0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
	state.enemies[0].isActive = true;

	state.enemies[1].position = glm::vec3(9.0, -13, 0);
	state.enemies[1].movement = glm::vec3(0);
	state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[1].isActive = true;
}