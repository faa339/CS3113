#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

void Entity::Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemycount)
{
    if (entityType == ENEMY)
        AI(player);

    collidedTop = false;
    collidedBot = false;
    collidedLeft = false;
    collidedRight = false;
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
        collidedBot = false;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    if(platforms != NULL)
        CheckCollisionsY(platforms, platformCount);// Fix if needed
    if (enemies != NULL)
        CheckCollisionsY(enemies, enemycount);

    position.x += velocity.x * deltaTime; // Move on X
    if(platforms != NULL)
        CheckCollisionsX(platforms, platformCount);// Fix if needed
    if (enemies != NULL)
        CheckCollisionsX(enemies, enemycount);

    if (animRows != 0 && animCols != 0) {
        if (movement.x > 0) {
            if (collidedBot)
                animIndex = 0;
            else
                animIndex = 2;
        }
        if (movement.x < 0) {
            if (collidedBot)
                animIndex = 1;
            else
                animIndex = 3;
        }

        if (movement.x == 0) {
            if (left) {
                if (collidedBot == false)
                    animIndex = 3;
                else
                    animIndex = 1;
            }
            if (right) {
                if (collidedBot == false)
                    animIndex = 2;
                else
                    animIndex = 0;
            }
        }
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (entityType == PLAYER) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndex);
        return;
    }

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

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

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
            if (object->entityType == ENEMY) {
                if (velocity.y >= 0) {
                    endBad = true;
                    return;
                }
                if (velocity.y < 0) {
                    object->isActive = false;
                    return;
                }
            }
                
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBot = true;
            }

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
            if (object->entityType == ENEMY) {
                endBad = true;
                return;
            }

            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void Entity::AI(Entity* player) {
    
    movement = glm::vec3(0);

    switch (aiType) {
    case CHASER: 
        AIChaser(player);
        break;

    case JUMPER:
        AIJumper();
        break;

    case PATROLLER:
        AIPatroller(player);
        break;
    }
}

void Entity::AIChaser(Entity* player) {
    if (player->position.x < position.x)
        movement.x = -1;
    else
        movement.x = 1;
}

void Entity::AIJumper() {
    if(collidedBot)
        jump = true;
}

void Entity::AIPatroller(Entity* player) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 2.5) {
            aiState = MOVING;
            movement.x = 1;
        }
        break;

    case MOVING:
        //This is a bit hacky but I'll be honest -- I was a little late 
        //and couldn't figure out exactly how to implement the sensors for 
        //pit detection without rewriting a lot
        movement.x = 1;
        if(position.x >= (startPos.x + patrolRange) || position.x <= startPos.x)
            speed *= -1;
            
        break;
    }
}