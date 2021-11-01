//
// Created by Eli on 01.11.2021.
//

#ifndef CATFOODIDLE_FOOD_OBJECT_H
#define CATFOODIDLE_FOOD_OBJECT_H


#include "game_object.h"

class FoodObject : public GameObject {
public:
    // food vars
    int UStatus;
    Texture2D Texture;
    float Value;

    bool ACExist = false;
    float ACSpeed;

    // constructor(s)
    FoodObject();
    // upgrades food texture
    void Upgrade();

};


#endif //CATFOODIDLE_FOOD_OBJECT_H
