//
// Created by Eli on 30.10.2021.
//

#include "fbox_object.h"
FBoxObject::FBoxObject()
        : GameObject(), UStatus(0), UCosts(30), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(0.0f), Shaking(false){ }

FBoxObject::FBoxObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, int initPosX, int initPosY)
        : GameObject(pos, size, sprite, glm::vec3(1.0f)), UStatus(0), UCosts(30), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(0.0f), Shaking(false), InitPosX(initPosX), InitPosY(initPosY) { }

void FBoxObject::Upgrade(int id)
{
    switch (id) {
        case 0:
            FoodDrop = FoodDrop * 2;
            break;
        case 1:
            if (FoodValue != 1){
                FoodValue = FoodValue + round(FoodValue * 0.5);
            } else {
                FoodValue++;
            }
            break;
        case 2:
            if (ACExist){
                ACExist = true;
                ACSpeed = 2;
            } else {
                ACSpeed -= 0.5;
            }
            break;
    }


}

glm::vec2 FBoxObject::Shake() {
    if(this->Shaking){
        if(this->Position.x < this->InitPosX + 10){
            Position.x++;
            Position.y--;
        } else if (this->Position.x >= this->InitPosX + 10){
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
