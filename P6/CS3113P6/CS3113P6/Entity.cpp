#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
    dirVec = glm::vec3(0);
    currentBullet = 0;
    animCols = 0;
    animRows = 0;
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map)
{
    if (isActive == false) return;


    if (entityType == ENEMY) {
        AI(player, deltaTime);
    }

    collidedTop = false;
    collidedBot = false;
    collidedLeft = false;
    collidedRight = false;

    velocity = movement * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(map);
    if(objects != NULL)
        CheckCollisionsY(objects, objectCount); // Fix if needed

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(map);
    if(objects != NULL)
        CheckCollisionsX(objects, objectCount); // Fix if needed

    
    if (reloading) {
        reloadingClock-= deltaTime;
        if (reloadingClock <= 0) {
            reloadingClock = reloadTime;
            currentBullet = 0;
            reloading = false;
        }
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, lookRotation, glm::vec3(0, 0, 1));
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

void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animCols != 0 && animRows != 0) {
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
            if (object->entityType == BULLET) {
                if (entityType == ENEMY) {
                    if (aiType == BOSS && canHit == true) {
                        BossHP--;
                        object->isActive = false;
                        if (BossHP == 0)
                            isActive = false;
                        return;
                    }
                    isActive = false;
                    object->isActive = false;
                    return;
                }
            }

            if (entityType == BULLET && object->entityType == PLAYER) {
                isActive = false;
                object->gotHit = true;
            }else if (object->entityType == ENEMY) {
                gotHit = true;
            }
            else {
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
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            if (object->entityType == BULLET) {
                if (entityType == ENEMY) {
                    if (aiType == BOSS && canHit == true) {
                        BossHP--;
                        object->isActive = false;
                        if (BossHP == 0)
                            isActive = false;
                        return;
                    }
                    isActive = false;
                    object->isActive = false;
                    return;
                }
            }

            //This part is a bit convoluted -- essence is, player will only 
            //register collisions with enemies but bullets can register collision
            //with player, so you have to switch perspective mid-statement for it
            //Could be fixed by changing update to check collisions against bullets 
            //too but this way works fine
            if (entityType == BULLET && object->entityType == PLAYER) {
                isActive = false;
                object->gotHit = true;
            }else if (object->entityType == ENEMY) {
                gotHit = true;
            }
            else {
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
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBot = true;
    }
  
    if ((collidedTop || collidedBot) && entityType == BULLET)
        isActive = false;
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
    if ((collidedLeft || collidedRight) && entityType == BULLET)
        isActive = false;

}

void Entity::Shoot() {
    if (currentBullet < maxAmmo && reloading==false) {
        Bullets[currentBullet].isActive = true;
        Bullets[currentBullet].position = position;
        Bullets[currentBullet].movement = dirVec;
        Bullets[currentBullet].lookRotation = lookRotation;
        currentBullet++;
        if(entityType == PLAYER)
            Mix_PlayChannel(-1, shotSound, 0);
    }
    else {
        currentBullet = 0;
        if(reloading == false)reloading = true;
    }
}

void Entity::AIDasher(Entity* player, float deltaTime) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 5.0f) {
            TurnHelperAI(player);
            Mix_PlayChannel(-1, dasherSound, 0);
            aiState = DASHING;
        }
        break;
    case DASHING:
        if (glm::distance(position, savedPoint) < 0.1) {
            if (waitTime > 0) {
                waitTime -= deltaTime;
                break;
            }
            else {
                waitTime = 0.8f;
                if (glm::distance(position, player->position) < 5.0f) {
                    Mix_PlayChannel(-1, dasherSound, 0);
                    TurnHelperAI(player);
                    movement = dirVec;
                    break;
                }
                else {
                    movement = glm::vec3(0);
                    aiState = IDLE;
                }
            }
        }
        else {
            if (collidedBot || collidedTop || collidedRight || collidedLeft) {
                //Manual resets if they hit a wall -- this will allow them to recalibrate
                //and adjust to where they should go. It's not really pathfinding, but its 
                // "wall bumping handling," I guess
                if (collidedBot && dirVec.y <= 0) { movement = glm::vec3(0); savedPoint = position;  waitTime = 0.8f; }
                if (collidedTop && dirVec.y >= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 0.8f; }
                if (collidedLeft && dirVec.x <= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 0.8f; }
                if (collidedRight && dirVec.x >= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 0.8f; }
            }
            else {

                movement = dirVec;
            }
        }
    }
}

void Entity::AIChaser(Entity* player) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 10.0f)
            aiState = WALKING;
        break;
    case WALKING:
        TurnHelperAI(player);
        movement = dirVec;
        break;
    }
}

void Entity::AIShooter(Entity* player, float deltaTime) {
    switch (aiState) {
    case IDLE: 
        if (glm::distance(position, player->position) < 12.0f) {
            TurnHelperAI(player);
            aiState = SHOOTING;
        }
        break;
    case SHOOTING:
        TurnHelperAI(player);
        if (shootSpacing > 0) {
            shootSpacing -= deltaTime;
        }
        else {
            shootSpacing = 1.5f;
            Shoot();
        }
        break;
    }
}

void Entity::AIBoss(Entity* player, float deltaTime) {
    switch (aiState) {
    case IDLE:
        if (glm::distance(position, player->position) < 6) {
            TurnHelperAI(player);
            Mix_PlayChannel(-1, dasherSound, 0);
            aiState = DASHING;
        }
        break;
    case DASHING:
        if (dashCount < 3) {
            if (glm::distance(savedPoint, position) < 0.1) {
                if (waitTime > 0) {
                    waitTime -= deltaTime;
                }
                else {
                    waitTime = 1.2f;
                    TurnHelperAI(player);
                    Mix_PlayChannel(-1, dasherSound, 0);
                    movement = dirVec;
                    dashCount++;
                }
            }
            else {
                if (collidedBot || collidedTop || collidedRight || collidedLeft) {
                    //Same as the dasher's resets
                    if (collidedBot && dirVec.y <= 0) { movement = glm::vec3(0); savedPoint = position;  waitTime = 1.2f; }
                    if (collidedTop && dirVec.y >= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 1.2f; }
                    if (collidedLeft && dirVec.x <= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 1.2f; }
                    if (collidedRight && dirVec.x >= 0) { movement = glm::vec3(0); savedPoint = position; waitTime = 1.2f; }
                }
                else {
                    movement = dirVec;
                }
            }
        }
        else {
            dashCount = 0;
            aiState = SHOOTING;
        }
        break;
    case SHOOTING:
        TurnHelperAI(player);
        if (shotCount < 9) {
            if (shootSpacing > 0) {
                shootSpacing -= deltaTime;
            }
            else {
                shootSpacing = 0.3f;
                Shoot();
                Mix_PlayChannel(-1, shotSound, 0);
                shotCount++;
            }
        }
        else{
            shotCount = 0;
            aiState = DASHING;
        }
        break;
    }
}


void Entity::AI(Entity* player, float deltaTime) {
    movement = glm::vec3(0);
    switch (aiType) {
    case CHASER:
        AIChaser(player);
        break;
    case DASHER:
        AIDasher(player, deltaTime);
        break;
    case SHOOTER:
        AIShooter(player, deltaTime);
        break;
    case BOSS:
        AIBoss(player, deltaTime);
        break;
    }
}



void Entity::TurnHelperAI(Entity* player) {
    savedPoint = player->position;
    float pointXDist = savedPoint.x - position.x;
    float pointYDist = savedPoint.y - position.y;
    dirVec = glm::normalize(glm::vec3(pointXDist, pointYDist, 0));
    lookRotation = glm::atan(dirVec.y, dirVec.x);
}