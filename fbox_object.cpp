//
// Created by Eli on 30.10.2021.
//

#include "fbox_object.h"
FBoxObject::FBoxObject()
        : GameObject(), UCosts(30), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(0), Shaking(false){ }

FBoxObject::FBoxObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
        : GameObject(pos, size, sprite, glm::vec3(1.0f), -120.0f), UCosts(30), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(0), Shaking(false){
    InitPosX = pos.x;
    InitPosY = pos.y;
}

void FBoxObject::Upgrade(int id)
{
    switch (id) {
        case 0:
            FoodDrop++;
            break;
        case 1:
            FoodValue++;
            break;
        case 2:
            if (!ACExist){
                ACExist = true;
                ACSpeed = 1;
            } else {
                ACSpeed++;
            }
            break;
    }


}

glm::vec2 FBoxObject::Shake() {
    if(this->Shaking){
        if(this->Position.x < this->InitPosX + 20){
            this->Position.x++;
            this->Position.y--;
        } else if (this->Position.x >= this->InitPosX + 20){
            this->Position.x = this->InitPosX;
            this->Position.y = this->InitPosY;
            this->setShaking(false);
        }
    }
    return this->Position;
}

float FBoxObject::DoAC() {
    return 0;
}

void FBoxObject::setShaking(bool v) {
    this->Shaking = v;
}
