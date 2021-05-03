#pragma once

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
#include "Map.h"
#include "SDL_mixer.h"

enum EntityType {PLAYER, PLATFORM, ENEMY, BULLET};

enum AIType { CHASER, DASHER, SHOOTER, BOSS};
enum AIState { IDLE, WALKING, DASHING, SHOOTING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    glm::vec3 dirVec;

    glm::vec3 savedPoint; //For enemies

    Entity* Bullets = NULL;
    int maxAmmo;
    int currentBullet;
    int BossHP;
    float waitTime;
    float shootSpacing;

    int dashCount;
    int shotCount;

    float speed;
    float jumpPower = 0;
    float lookRotation = 0;

    bool isActive = true;

    bool collidedTop = false;
    bool collidedBot = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    bool left=false;
    bool right=false;

    bool endGood;
    bool canHit;
    bool gotHit;
    bool reloading = false;
    float reloadTime;
    float reloadingClock;

    GLuint textureID;

    Mix_Chunk* shotSound;
    Mix_Chunk* dasherSound;

    glm::mat4 modelMatrix;

    int animIndex = 0;
    int animCols = 0;
    int animRows = 0;
    
    float width = 1;
    float height = 1;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    void Shoot();
    void AI(Entity* player, float deltaTime);
    void AIDasher(Entity* player, float deltaTime);
    void AIShooter(Entity* player, float deltaTime);
    void AIChaser(Entity* player);
    void AIBoss(Entity* player, float deltaTime);
    void TurnHelperAI(Entity* player);
};