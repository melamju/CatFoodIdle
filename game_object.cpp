//
// Created by Eli on 27.10.2021.
//

#include "game_object.h"
#include <iostream>

GameObject::GameObject()
        : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false),
          Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, float rot, glm::vec2 velocity)
        : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(rot), Sprite(sprite), IsSolid(false),
          Destroyed(false) {}

void GameObject::Draw(SpriteRenderer &renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

bool GameObject::CheckCollisionMouse(double x, double y) {
    //https://stackoverflow.com/questions/37494364/simple-way-to-determine-what-object-a-mouse-is-pointed-over-opengl-freeglut
    // The mouse is inside if the coordinates are 'inside' all walls

    return (x > this->Position.x &&
            y > this->Position.y &&
            x < this->Position.x + this->Size.x &&
            y < this->Position.y + this->Size.y);

}