//
// Created by Eli on 21.10.2021.
//

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "fbox_object.h"
#include <iostream>

SpriteRenderer  *Renderer;

// mouse state
static int oldState = GLFW_RELEASE;
double mouseX;
double mouseY;

// game objects
int Money = 0;
FBoxObject *Box;

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
    ResourceManager::LoadShader(R"(C:\Users\eliev\CLionProjects\CatFoodIdle\shaders\sprite.vs)", R"(C:\Users\eliev\CLionProjects\CatFoodIdle\shaders\sprite.fs)", nullptr, "sprite");

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
    ResourceManager::LoadTexture(R"(C:\Users\eliev\CLionProjects\CatFoodIdle\img\background.png)", true, "bg");
    ResourceManager::LoadTexture(R"(C:\Users\eliev\CLionProjects\CatFoodIdle\img\cat1.png)", true, "box");

    // load objects
    Box = new FBoxObject(glm::vec2(200, 200), glm::vec2(300,300), ResourceManager::GetTexture("box"), 200, 200);

}

void Game::Update(float dt)
{
    Box->Shake();
}

void Game::ProcessInput(float dt, GLFWwindow* window)
{
    if (this->State == GAME_ACTIVE) {
        //https://community.khronos.org/t/how-do-i-detect-only-1-mouse-press-with-glfw/75132/2
        int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS && Box->CheckCollisionMouse(mouseX, mouseY))
            {
                if (!(Box->Shaking)) Box->setShaking(true);
            }
        oldState = newState;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE) {
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("bg");
        Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(800, 900), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        //position, height & width, color
        Box->Draw(*Renderer);
        glFinish();
    }
}