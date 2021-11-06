//
// Created by Eli on 30.10.2021.
//

#include "fbox_object.h"
FBoxObject::FBoxObject()
        : GameObject(), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(7), Shaking(false){ }

FBoxObject::FBoxObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
        : GameObject(pos, size, sprite, glm::vec3(1.0f), -120.0f), FoodDrop(1), FoodValue(1), ACExist(false), ACSpeed(7), Shaking(false){
    InitPosX = pos.x;
    InitPosY = pos.y;
}

unsigned int FBoxObject::Upgrade(int id, unsigned int money)
{
    switch (id) {
        default:
            return 0;

        case 0:
            if (money >= CFoodDrop) {
                FoodDrop++;
                unsigned int saveCost = CFoodDrop;
                CFoodDrop = CFoodDrop * 3;
                return saveCost;
            }
            break;
        case 1:
            if (money >= CFoodValue) {
                FoodValue++;
                unsigned int saveCost = CFoodValue;
                CFoodValue = CFoodValue * 3;
                return saveCost;
            }
            break;
        case 2:
            if (money >= CAC) {
                if (!ACExist) {
                    ACExist = true;
                } else if (ACSpeed != 2) {
                    ACSpeed--;
                }
                unsigned int saveCost = CAC;
                CAC = CAC * 3;
                return saveCost;
            }
            break;
    }
    return 0;
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


void FBoxObject::setShaking(bool v) {
    this->Shaking = v;
}


