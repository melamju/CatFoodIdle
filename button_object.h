//
// Created by Eli on 06.11.2021.
//

#ifndef CATFOODIDLE_BUTTON_OBJECT_H
#define CATFOODIDLE_BUTTON_OBJECT_H


#include "game_object.h"

class ButtonObject : public GameObject {
public:
    // button var
    int IDButton;

    // constructor(s)
    ButtonObject();
    ButtonObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, int id);

    // functions
};


#endif //CATFOODIDLE_BUTTON_OBJECT_H
