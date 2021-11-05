//
// Created by Eli on 01.11.2021.
//

#include "food_object.h"
#include <iostream>

FoodObject::FoodObject()
    :GameObject() {
    Destroyed = true;
}

FoodObject::FoodObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float rot)
    :GameObject(pos, size, sprite) {
    Destroyed = true;
    Rotation = rot;
    Velocity = glm::vec2 (-350.0f, 350.0f);
    InitPosX = pos.x;
    InitPosY = pos.y;
}

glm::vec2 FoodObject::Move(float dt) {
    if (!this->Destroyed){
        this->Position += Velocity * dt;
        Velocity.x += 1;
        Velocity.y += 5;
    } else {
        Velocity = glm::vec2 (-350.0f, 350.0f);
        Position.x = InitPosX;
        Position.y = InitPosY;
    }
    return this->Position;
}
