//
// Created by Eli on 21.10.2021.
//

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "fbox_object.h"
#include "food_object.h"
#include "cat_object.h"
#include <vector>
#include <iostream>

SpriteRenderer  *Renderer;

// mouse state
static int oldState = GLFW_RELEASE;
double mouseX;
double mouseY;

// game objects
int Money = 0;
FBoxObject *Box;
    // food array
    std::vector<FoodObject*> FoodArray = {};
    int IterFood = 0;
    // cat array
    std::vector<CatObject*> CatArray = {};
    int CatCount = 0;

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
    ResourceManager::LoadTexture("img/bowl-front.png", true, "bowl-front");
    ResourceManager::LoadTexture("img/bowl-back.png", true, "bowl-back");
    ResourceManager::LoadTexture("img/food.png", true, "food");
    ResourceManager::LoadTexture("img/cat1.png", true, "cat1");


    // load objects
    Box = new FBoxObject(glm::vec2(400, 100), glm::vec2(250,250), ResourceManager::GetTexture("box"));
    // load FoodObjects in array
    for(int i = 0; i < 10; i++){
        //420,330 max
        //400,230 min rand spawn pos
        int ranX = rand() % 20 + 400;
        int ranY = rand() % 100 + 230;
        auto *fo = new FoodObject(glm::vec2(ranX, ranY), glm::vec2(50, 50), ResourceManager::GetTexture("food"), 45.0f);
        FoodArray.push_back(fo);
    }
    // load CatObjects in array
    for(int i = 0; i < 5; i++) {
        int ranY = rand() % 50 + 600; //rand spawn pos
        auto *co = new CatObject(glm::vec2(800, ranY), glm::vec2(200, 200), ResourceManager::GetTexture("cat1"));
        CatArray.push_back(co);
    }
}

void Game::Update(float dt)
{
    Box->Shake();
    //food moving
    for (auto &f : FoodArray) {
        if (f->Position.y < 670) {
            f->Move(dt);
        } else {
            f->Destroyed = true;
            f->Move(dt);
        }
    }

    //cats spawning
    CatSpawn();

    //cats moving
    for (auto &c : CatArray) {
        if (c->Position.x > 0 - (c->Size.x)) {
            c->Move(dt);
        } else {
            c->Destroyed = true;
            c->Move(dt);
        }
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

                if (IterFood <= FoodArray.size()-1) {
                    FoodArray[IterFood]->Destroyed = false;
                    if (IterFood!=9){
                        IterFood++;
                    } else {
                        IterFood = 0;
                    }
                }
            }
        oldState = newState;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE) {
        Texture2D myTexture;

        // draw background
        myTexture = ResourceManager::GetTexture("bg");
        Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(800, 900), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("bowl-back");
        Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));


        // draw objects
        Box->Draw(*Renderer);
        glFinish();

        for (auto &f : FoodArray) {
            if (!f->Destroyed) f->Draw(*Renderer);
        }
        glFinish();

        // draw food bowl "in front of" food
        myTexture = ResourceManager::GetTexture("bowl-front");
        Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        // draw cats
        for(auto &c : CatArray) {
            if(!c->Destroyed) c->Draw(*Renderer);
        }
        glFinish();

    }
}

void Game::CatSpawn(){
    if (Money < 10) {
        CatCount = 0;
    }
    if (Money >= 10) {
        CatCount = 1;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 20) {
        CatCount = 2;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 30) {
        CatCount = 3;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 40) {
        CatCount = 4;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 50) {
        CatCount = 5;
        CatArray[CatCount - 1]->Destroyed = false;
    }
}