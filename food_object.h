//
// Created by Eli on 01.11.2021.
//

#ifndef CATFOODIDLE_FOOD_OBJECT_H
#define CATFOODIDLE_FOOD_OBJECT_H


#include "game_object.h"

class FoodObject : public GameObject {
public:
    // food var
    int InitPosX, InitPosY, Timer;

    // constructor(s)
    FoodObject();
    FoodObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float rot);

    // functions
    glm::vec2 Move(float dt);

};


#endif //CATFOODIDLE_FOOD_OBJECT_H
