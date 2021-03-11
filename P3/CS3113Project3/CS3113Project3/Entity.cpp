#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

void Entity::Update(float deltaTime, Entity* platforms, int platformCount)
{
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    //position.x += velocity.x * deltaTime;
    if(platforms != NULL)
        CheckCollisionsY(platforms, platformCount);// Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
    if(platforms != NULL)
        CheckCollisionsX(platforms, platformCount);// Fix if needed

    if (endBad == false && endGood == false) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
    }
}


void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0)
        return true;
    else
        return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            if (object->entityType == BADPLATFORM)
                endBad = true;
            if(object->entityType == GOODPLATFORM)
                endGood = true;
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            if (object->entityType == BADPLATFORM)
                endBad = true;
            if (object->entityType == GOODPLATFORM)
                endGood = true;
        }
    }
}