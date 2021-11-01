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
    int UStatus, FoodDrop, UCosts, FoodValue;
    Texture2D Texture;

    //Auto Clicker
    bool ACExist = false;
    float ACSpeed;

    // constructor(s)
    FBoxObject();
    // upgrades foodbox texture
    void Upgrade(int id);
    float DoAC();
};


#endif //CATFOODIDLE_FBOX_OBJECT_H
