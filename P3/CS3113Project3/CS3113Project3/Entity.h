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

enum eType{ PLAYER, BADPLATFORM, GOODPLATFORM};


class Entity {
    //TODO: Add rotation variable for left/right
public:
    glm::vec3 position;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float speed;

    eType entityType;

    bool isActive;
    
    bool endGood = false;
    bool endBad = false;

    GLuint textureID;

    glm::mat4 modelMatrix;

    float width = 1;
    float height = 1;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void Update(float deltaTime, Entity* platforms, int platformCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};