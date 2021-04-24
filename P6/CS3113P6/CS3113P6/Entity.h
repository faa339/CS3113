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

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType { CHASER, JUMPER };
enum AIState { IDLE, WALKING };

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

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
    bool gotHit;

    GLuint textureID;

    Mix_Chunk* killSound;

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
    void AI(Entity* player);
    void AIJumper();
    void AIChaser(Entity* player);
};