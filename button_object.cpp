//
// Created by Eli on 06.11.2021.
//

#include "button_object.h"

ButtonObject::ButtonObject()
    : GameObject(){

}

ButtonObject::ButtonObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, int id)
    : GameObject(pos, size, sprite){
    IDButton = id;

}



