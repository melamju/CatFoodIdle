//
// Created by Eli on 30.10.2021.
//

#ifndef CATFOODIDLE_FBOX_OBJECT_H
#define CATFOODIDLE_FBOX_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

class FBoxObject : public GameObject {
public:
    // foodbox vars
    unsigned int FoodDrop, FoodValue, InitPosX, InitPosY;
    bool Shaking;

    // auto clicker
    bool ACExist;
    int ACSpeed;

    // Upgrade costs
    unsigned int CFoodDrop = 15, CFoodValue = 30, CAC = 20;

    // constructor(s)
    FBoxObject();
    FBoxObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

    // functions
    unsigned int Upgrade(int id, unsigned int money);
    glm::vec2 Shake();
    void setShaking(bool v);
};


#endif //CATFOODIDLE_FBOX_OBJECT_H
