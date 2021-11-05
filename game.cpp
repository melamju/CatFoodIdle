//
// Created by Eli on 21.10.2021.
//

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "fbox_object.h"
#include "food_object.h"
#include <iostream>

SpriteRenderer  *Renderer;

// mouse state
static int oldState = GLFW_RELEASE;
double mouseX;
double mouseY;

// game objects
int Money = 0;
FBoxObject *Box;
FoodObject *Food [1];

Game::Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);

    // load textures
    ResourceManager::LoadTexture("img/background.png", true, "bg");
    ResourceManager::LoadTexture("img/box.png", true, "box");
    ResourceManager::LoadTexture("img/bowl.png", true, "bowl");
    ResourceManager::LoadTexture("img/food.png", true, "food");

    // load objects
    Box = new FBoxObject(glm::vec2(400, 100), glm::vec2(250,250), ResourceManager::GetTexture("box"));
    // load FoodObjects in array
    FoodObject *fo = new FoodObject(glm::vec2(400, 350), glm::vec2(50, 50), ResourceManager::GetTexture("food"), 45.0f);
    Food[0] = fo;
    std::cout << Food[0];
}

void Game::Update(float dt)
{
    Box->Shake();
    if (Food[0]->Timer < 180){
        Food[0]->Move(dt);
    }
}

void Game::ProcessInput(float dt, GLFWwindow* window)
{
    if (this->State == GAME_ACTIVE) {
        //https://community.khronos.org/t/how-do-i-detect-only-1-mouse-press-with-glfw/75132/2
        int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS && Box->CheckCollisionMouse(mouseX, mouseY))
            {
                Money++;
                if (!(Box->Shaking)) Box->setShaking(true);
                Food[0]->Destroyed=(false);
            }
        oldState = newState;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE) {
        Texture2D myTexture;

        // draw not-objects
        myTexture = ResourceManager::GetTexture("bg");
        Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(800, 900), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("bowl");
        Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        // draw objects
        Box->Draw(*Renderer);
        glFinish();

        if (!Food[0]->Destroyed) Food[0]->Draw(*Renderer);
        glFinish();
    }
}