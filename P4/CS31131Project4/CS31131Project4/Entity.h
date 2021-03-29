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

enum eType{ PLAYER, PLATFORM, ENEMY};
enum AIType{CHASER, JUMPER, PATROLLER};
enum AIState {IDLE, MOVING};

class Entity {

public:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    //For patroller-type AI
    glm::vec3 startPos;
    float patrolRange; 
    //End of patroller-specific stuff


    float speed;
    float jumpPower = 0;

    eType entityType;
    AIType aiType;
    AIState aiState;


    bool isActive;
    
    bool endGood = false;
    bool endBad = false;
    bool collidedTop = false;
    bool collidedBot = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    bool jump = false;

    bool left = false;
    bool right = false;


    GLuint textureID;

    glm::mat4 modelMatrix;

    float width = 1;
    float height = 1;

    int animFrames = 0;
    int animIndex = 0;
    int animCols = 0;
    int animRows = 0;
    Mix_Chunk* killSound;


    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemycount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    void AI(Entity* player);
    void AIChaser(Entity* player);
    void AIJumper();
    void AIPatroller(Entity* player);
};