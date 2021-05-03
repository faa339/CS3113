#include "Level3.h"
#define LEVEL3_WIDTH 30
#define LEVEL3_HEIGHT 30
#define ENEMY_COUNT 1
#define WALL_COUNT 10

unsigned int level3_data[] =
{
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,4,4,4,4,4,6,5,5,5,5,5,5,5,5,5,5,6,4,4,4,4,4,0,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,0,0,0,
	0,0,0,0,3,3,3,3,3,3,3,6,5,5,5,5,5,5,6,3,3,3,3,3,3,3,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,2,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void Level3::Initialize() {
	state.nextScene = -1;
	bgModelMatrix = glm::mat4(1.0f);
	backGround = Util::LoadTexture("spaceBG.png", true);
	mapTexID = Util::LoadTexture("mapTiles.png", false);
	GLuint characters = Util::LoadTexture("characters.png", false);
	state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTexID, 1.0f, 8, 1);
	Mix_Chunk* gunshot = Mix_LoadWAV("gunShot_cjdeets.wav");
	Mix_Chunk* dashSound = Mix_LoadWAV("bossDasher.wav");
	Mix_Chunk* death = Mix_LoadWAV("playerDeath.wav");
	Mix_Chunk* bossDeath = Mix_LoadWAV("bossDeath.wav");
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->textureID = characters;
	state.player->animRows = 1;
	state.player->animCols = 6;
	state.player->animIndex = 0;
	state.player->position = glm::vec3(14.4f, -0.2f, 0);
	state.player->speed = 4.75f;
	state.player->height = 0.6;
	state.player->width = 0.2;
	state.player->maxAmmo = 12;
	state.player->reloadTime = 2.0f;
	state.player->reloadingClock = 2.0f;
	state.player->Bullets = new Entity[state.player->maxAmmo];
	state.player->shotSound = gunshot;
	state.player->deathSound = death;
	for (int i = 0; i < state.player->maxAmmo; i++) {
		state.player->Bullets[i].entityType = BULLET;
		state.player->Bullets[i].speed = 9.5f;
		state.player->Bullets[i].textureID = characters;
		state.player->Bullets[i].animRows = 1;
		state.player->Bullets[i].animCols = 6;
		state.player->Bullets[i].animIndex = 5;
		state.player->Bullets[i].width = 0.2;
		state.player->Bullets[i].height = 0.2;
		state.player->Bullets[i].isActive = false;
	}
	state.player->isActive = true;

	state.enemies = new Entity[ENEMY_COUNT];
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].aiType = BOSS;
	state.enemies[0].aiState = IDLE;
	state.enemies[0].textureID = characters;
	state.enemies[0].animRows = 1;
	state.enemies[0].animCols = 6;
	state.enemies[0].animIndex = 4;
	state.enemies[0].speed = 7.5f;
	state.enemies[0].dashCount = 0;
	state.enemies[0].shotCount = 0;
	state.enemies[0].shootSpacing = 0;
	state.enemies[0].maxAmmo = 9;
	state.enemies[0].reloadTime = 0.5f;
	state.enemies[0].reloadingClock = 0.5f;
	state.enemies[0].waitTime = 1.2f;
	state.enemies[0].Bullets = new Entity[state.enemies[0].maxAmmo];
	state.enemies[0].shotSound = gunshot;
	state.enemies[0].dasherSound = dashSound;
	state.enemies[0].deathSound = bossDeath;
	for (int i = 0; i < state.enemies[0].maxAmmo; i++) {
		state.enemies[0].Bullets[i].entityType = BULLET;
		state.enemies[0].Bullets[i].speed = 9.0f;
		state.enemies[0].Bullets[i].textureID = characters;
		state.enemies[0].Bullets[i].animRows = 1;
		state.enemies[0].Bullets[i].animCols = 6;
		state.enemies[0].Bullets[i].animIndex = 5;
		state.enemies[0].Bullets[i].width = 0.2;
		state.enemies[0].Bullets[i].height = 0.2;
		state.enemies[0].Bullets[i].isActive = false;
	}
	state.enemies[0].position = glm::vec3(14.5f, -25.5f, 0);
	state.enemies[0].canHit = false;
	state.enemies[0].BossHP = 36;
	state.enemies[0].height = 1.5;
	state.enemies[0].width = 1.5;
	state.enemies[0].isActive = true;
};



void Level3::Update(float deltaTime) {
	state.enemies[0].Update(deltaTime, state.player, state.player->Bullets, state.player->maxAmmo, state.map);
	for (int j = 0; j < state.enemies[0].maxAmmo; j++)
		state.enemies[0].Bullets[j].Update(deltaTime, state.player, state.player, 1, state.map);
	state.enemies[0].modelMatrix = glm::scale(state.enemies[0].modelMatrix, glm::vec3(1.5f, 1.5f, 1));

	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player->gotHit) {
		Mix_PlayChannel(-1, state.player->deathSound, 0);
		state.player->gotHit = false;
		Reset();
	}
		
	if (state.player->position.y < -19.0f && mapSwitch==false) {
		for (int i = 10; i < 20; i++) {
			level3_data[i + (18*30)] = 7;
		}
		state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTexID, 1.0f, 8, 1);
		mapSwitch = true;
		state.enemies[0].canHit = true;
	}

	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].Update(deltaTime, state.player, NULL, 0, state.map);

	if (state.enemies[0].BossHP == 0) {
		Mix_PlayChannel(-1, state.enemies[0].deathSound, 0);
		state.player->endGood = true;
	}
};



void Level3::Render(ShaderProgram* program) {
	RenderBG(program);
	state.map->Render(program);

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Render(program);
		if (state.enemies[i].Bullets != NULL) {
			for (int j = 0; j < state.enemies[i].maxAmmo; j++)
				state.enemies[i].Bullets[j].Render(program);
		}
	}

	state.player->Render(program);
	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].Render(program);

};

void Level3::RenderBG(ShaderProgram* program) {
	//Save some space by only rendering the background, instead of adding tiles to the map!
	program->SetModelMatrix(bgModelMatrix);

	float vertices[] = { -40.5, -40.5,
						  40.5, -40.5,
						  40.5,  40.5,
						 -40.5, -40.5,
						  40.5,  40.5,
						 -40.5, 40.5 };
	float texCoords[] = { 0.0, 40.0, 40.0, 40.0, 40.0, 0.0, 0.0, 40.0, 40.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, backGround);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Level3::Reset() {
	state.player->position = glm::vec3(14.4f, -0.2f, 0);
	state.player->movement = glm::vec3(0);
	state.player->currentBullet = 0;
	state.player->reloading = false;
	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].isActive = false;


	state.enemies[0].position = glm::vec3(14.0f, -26.0f, 0);
	state.enemies[0].aiState = IDLE;
	state.enemies[0].movement = glm::vec3(0);
	state.enemies[0].canHit = false;
	state.enemies[0].currentBullet = 0;
	state.enemies[0].reloading = false;
	state.enemies[0].BossHP = 36;
	if (mapSwitch == true) {
		for (int i = 10; i < 20; i++) {
			level3_data[i + (18 * 30)] = 5;
		}
		state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTexID, 1.0f, 8, 1);
		mapSwitch = false;
	}

}