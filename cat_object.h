//
// Created by Eli on 05.11.2021.
//

#ifndef CATFOODIDLE_CAT_OBJECT_H
#define CATFOODIDLE_CAT_OBJECT_H


#include "game_object.h"

class CatObject : public GameObject {
public:
    // cat var
    float InitPosX, InitPosY;

    // constructor(s)
    CatObject();

    CatObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

    // functions
    glm::vec2 Move(float dt);

};


#endif //CATFOODIDLE_CAT_OBJECT_H
