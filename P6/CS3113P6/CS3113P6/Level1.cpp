#include "Level1.h"
#define LEVEL1_WIDTH 30
#define LEVEL1_HEIGHT 30
#define ENEMY_COUNT 26


unsigned int level1_data[] =
{
	0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,
	1,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,5,6,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,7,7,7,7,7,7,7,7,7,7,7,7,5,5,5,5,7,7,7,7,7,7,7,7,7,7,7,7,2,
	1,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,7,7,7,7,7,7,7,7,7,7,7,6,5,5,5,5,6,7,7,7,7,7,7,7,7,7,7,7,2,
	1,7,7,7,7,7,7,7,7,7,7,7,6,5,5,5,5,6,7,7,7,7,7,7,7,7,7,7,7,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2,
	1,5,5,5,5,5,5,5,5,5,5,5,6,5,5,5,5,6,5,5,5,5,5,5,5,5,5,5,5,2,
    1,7,7,7,7,7,7,7,7,7,7,7,6,5,5,5,5,6,7,7,7,7,7,7,7,7,7,7,7,2
};

void Level1::Initialize() {
	state.nextScene = -1;
	bgModelMatrix = glm::mat4(1.0f);
	backGround = Util::LoadTexture("spaceBG.png", true);
	state.font = Util::LoadTexture("font.png", false);
	GLuint mapTexID = Util::LoadTexture("mapTiles.png", false);
	GLuint characters = Util::LoadTexture("characters.png", false);
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTexID, 1.0f, 8, 1);
	Mix_Chunk* gunshot = Mix_LoadWAV("gunShot_cjdeets.wav");
	Mix_Chunk* dashSound = Mix_LoadWAV("dasher.wav");
	Mix_Chunk* death = Mix_LoadWAV("playerDeath.wav");
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->textureID = characters;
	state.player->animRows = 1;
	state.player->animCols = 6;
	state.player->animIndex = 0;
	state.player->height = 0.6;
	state.player->width = 0.2;
	state.player->position = glm::vec3(4.0f,-4.0f,0);
	state.player->speed = 4.75f;
	state.player->maxAmmo = 12;
	state.player->reloadTime = 2.0f;
	state.player->reloadingClock = 2.0f;
	state.player->shotSound = gunshot;
	state.player->deathSound = death;
	state.player->Bullets = new Entity[state.player->maxAmmo];
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
	for (int i = 0; i < 9; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = characters;
		state.enemies[i].animRows = 1;
		state.enemies[i].animCols = 6;
		state.enemies[i].animIndex = 1;
		state.enemies[i].aiType = CHASER;
		state.enemies[i].speed = 2.5f;
		state.enemies[i].aiState = IDLE;
		state.enemies[i].isActive = true;
	}
	state.enemies[0].position = glm::vec3(14.7f, -2.25f, 0);
	state.enemies[1].position = glm::vec3(26.0f, -2.25f, 0);
	state.enemies[2].position = glm::vec3(17.25f, -3.25f, 0);
	state.enemies[3].position = glm::vec3(22.25f, -3.25f, 0);
	state.enemies[4].position = glm::vec3(17.25f, -5.5f, 0);
	state.enemies[5].position = glm::vec3(23.75f, -7.5f, 0);
	state.enemies[6].position = glm::vec3(20.0f, -20.0f, 0);
	state.enemies[7].position = glm::vec3(25.0f, -21.0f, 0);
	state.enemies[8].position = glm::vec3(20.0f, -22.0f, 0);
	
	for (int i = 9; i < 18; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = characters;
		state.enemies[i].animRows = 1;
		state.enemies[i].animCols = 6;
		state.enemies[i].animIndex = 2;
		state.enemies[i].aiType = DASHER;
		state.enemies[i].speed = 5.5f;
		state.enemies[i].aiState = IDLE;
		state.enemies[i].dasherSound = dashSound;
		state.enemies[i].isActive = true;
	}
	state.enemies[9].position = glm::vec3(28.0f,-10.0f,0);
	state.enemies[10].position = glm::vec3(27.0f,-12.0f,0);
	state.enemies[11].position = glm::vec3(28.0f, -14.0f, 0);
	state.enemies[12].position = glm::vec3(13.0f,-11.0f,0);
	state.enemies[13].position = glm::vec3(19.0f,-11.0f,0);
	state.enemies[14].position = glm::vec3(15.0f,-13.0f,0);
	state.enemies[15].position = glm::vec3(17.0f,-17.0f, 0);
	state.enemies[16].position = glm::vec3(10.0f, -22.0f, 0);
	state.enemies[17].position = glm::vec3(8.0f, -23.0f, 0);

	for (int i = 18; i < ENEMY_COUNT; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].textureID = characters;
		state.enemies[i].animRows = 1;
		state.enemies[i].animCols = 6;
		state.enemies[i].animIndex = 3;
		state.enemies[i].aiType = SHOOTER;
		state.enemies[i].aiState = IDLE;
		state.enemies[i].maxAmmo = 3;
		state.enemies[i].reloadTime = 1.5f;
		state.enemies[i].reloadingClock = 1.5f;
		state.enemies[i].Bullets = new Entity[state.enemies[i].maxAmmo];
		state.enemies[i].shotSound = gunshot;
		for (int j = 0; j < state.enemies[i].maxAmmo; j++) {
			state.enemies[i].Bullets[j].entityType = BULLET;
			state.enemies[i].Bullets[j].speed = 8.5f;
			state.enemies[i].Bullets[j].textureID = characters;
			state.enemies[i].Bullets[j].animRows = 1;
			state.enemies[i].Bullets[j].animCols = 6;
			state.enemies[i].Bullets[j].animIndex = 5;
			state.enemies[i].Bullets[j].width = 0.2;
			state.enemies[i].Bullets[j].height = 0.2;
			state.enemies[i].Bullets[j].isActive = false;
		}
		state.enemies[i].isActive = true;
	}
	state.enemies[18].position = glm::vec3(4.0f, -11.0f, 0);
	state.enemies[19].position = glm::vec3(6.0f, -13.0f, 0);
	state.enemies[20].position = glm::vec3(9.0f,-14.0f,0);
	state.enemies[21].position = glm::vec3(27.0f,-16.0f,0);
	state.enemies[22].position = glm::vec3(7.0f, -17.0f, 0);
	state.enemies[23].position = glm::vec3(10.0f, -17.0f, 0);
	state.enemies[24].position = glm::vec3(13.0f, -27.0f, 0);
	state.enemies[25].position = glm::vec3(16.0f, -27.0f, 0);
};



void Level1::Update(float deltaTime) {
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.player->Bullets, state.player->maxAmmo, state.map);	
		if (state.enemies[i].Bullets != NULL) {
			for (int j = 0; j < state.enemies[i].maxAmmo; j++)
				state.enemies[i].Bullets[j].Update(deltaTime, state.player, state.player, 1, state.map);
		}
	}
	state.player->Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player->gotHit) {
		Mix_PlayChannel(-1, state.player->deathSound, 0);
		state.player->gotHit = false;
		Reset();
	}
	for (int i = 0; i < state.player->maxAmmo; i++)
		state.player->Bullets[i].Update(deltaTime, state.player, NULL, 0, state.map);

	if ((state.player->position.x >= 13.0f || state.player->position.x <= 16.0f) && state.player->position.y < -28)
		state.nextScene = 2;
	//SDL_Log("%f %f\n", state.player->position.x, state.player->position.y);
};



void Level1::Render(ShaderProgram* program) {
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

	Util::DrawText(program, state.font, "WASD to move!", 0.2, 0.02, glm::vec3(3.0f, -2.1f, 0));
	Util::DrawText(program, state.font, "Left Click to shoot!", 0.2, 0.02, glm::vec3(3.0f, -2.35f, 0));
	Util::DrawText(program, state.font, "Left Shift to peek!", 0.2, 0.02, glm::vec3(3.0f, -2.6f, 0));
	Util::DrawText(program, state.font, "R key to reload!", 0.2, 0.02, glm::vec3(3.0f, -2.85f, 0));
	Util::DrawText(program, state.font, "ESC to pause!", 0.2, 0.02, glm::vec3(3.0f, -3.1f, 0));
};

void Level1::RenderBG(ShaderProgram* program) {
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

void Level1::Reset() {
	//Reset player & enemies on enemy hit
	state.player->position = glm::vec3(4.0f, -4.0f, 0);
	state.player->movement = glm::vec3(0);
	state.player->currentBullet = 0;
	state.player->reloading = false;
	for (int i = 0; i < state.player->maxAmmo; i++) 
		state.player->Bullets[i].isActive = false;


	state.enemies[0].position = glm::vec3(14.7f, -2.25f, 0);
	state.enemies[1].position = glm::vec3(26.0f, -2.25f, 0);
	state.enemies[2].position = glm::vec3(17.25f, -3.25f, 0);
	state.enemies[3].position = glm::vec3(22.25f, -3.25f, 0);
	state.enemies[4].position = glm::vec3(17.25f, -5.5f, 0);
	state.enemies[5].position = glm::vec3(23.75f, -7.5f, 0);
	state.enemies[6].position = glm::vec3(20.0f, -20.0f, 0);
	state.enemies[7].position = glm::vec3(25.0f, -21.0f, 0);
	state.enemies[8].position = glm::vec3(20.0f, -22.0f, 0);
	for (int i = 0; i < 9; i++) {
		state.enemies[i].aiState = IDLE;
		state.enemies[i].isActive = true;
	}


	state.enemies[9].position = glm::vec3(28.0f, -10.0f, 0);
	state.enemies[10].position = glm::vec3(27.0f, -12.0f, 0);
	state.enemies[11].position = glm::vec3(28.0f, -14.0f, 0);
	state.enemies[12].position = glm::vec3(13.0f, -11.0f, 0);
	state.enemies[13].position = glm::vec3(19.0f, -11.0f, 0);
	state.enemies[14].position = glm::vec3(15.0f, -13.0f, 0);
	state.enemies[15].position = glm::vec3(17.0f, -17.0f, 0);
	state.enemies[16].position = glm::vec3(10.0f, -22.0f, 0);
	state.enemies[17].position = glm::vec3(8.0f, -23.0f, 0);
	for (int i = 9; i < 18; i++) {
		state.enemies[i].aiState = IDLE;
		state.enemies[i].isActive = true;
	}

	
	state.enemies[18].position = glm::vec3(4.0f, -11.0f, 0);
	state.enemies[19].position = glm::vec3(6.0f, -13.0f, 0);
	state.enemies[20].position = glm::vec3(9.0f, -14.0f, 0);
	state.enemies[21].position = glm::vec3(27.0f, -16.0f, 0);
	state.enemies[22].position = glm::vec3(7.0f, -17.0f, 0);
	state.enemies[23].position = glm::vec3(10.0f, -17.0f, 0);
	state.enemies[24].position = glm::vec3(13.0f, -27.0f, 0);
	state.enemies[25].position = glm::vec3(16.0f, -27.0f, 0);

	for (int i = 18; i < ENEMY_COUNT; i++) {
		state.enemies[i].currentBullet = 0;
		state.enemies[i].reloading = false;
		state.enemies[i].aiState = IDLE;
		state.enemies[i].isActive = true;
	}

}