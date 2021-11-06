//
// Created by Eli on 05.11.2021.
//

#include "cat_object.h"

CatObject::CatObject()
    : GameObject (){
    Destroyed = true;
}

CatObject::CatObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
    : GameObject(pos, size, sprite) {
    InitPosX = pos.x;
    InitPosY = pos.y;
    Destroyed = true;
    Velocity = glm::vec2 (-100.0f, 0);
}

glm::vec2 CatObject::Move(float dt) {
    if (!this->Destroyed){
        this->Position += Velocity * dt;
    } else {
        Position.x = InitPosX;
        Position.y = InitPosY;
    }
    return this->Position;
}
