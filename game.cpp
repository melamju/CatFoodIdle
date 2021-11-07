//
// Created by Eli on 21.10.2021.
//

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "fbox_object.h"
#include "food_object.h"
#include "cat_object.h"
#include "button_object.h"
#include <vector>
#include <iostream>
#include <string>

SpriteRenderer *Renderer;
double dtTracker;

// mouse state
static int oldState = GLFW_RELEASE;
double mouseX;
double mouseY;

// game objects
unsigned int Money = 0;
FBoxObject *Box;
GameObject *FootPrint;
// food array
std::vector<FoodObject *> FoodArray = {};
int IterFood = 0;
// cat array
std::vector<CatObject *> CatArray = {};
int CatCount = 0;
// button array
std::vector<ButtonObject *> BArray = {};

Game::Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {
    delete Renderer;
}

void Game::Init() {
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
    loadAllTextures();

    // load objects
    Box = new FBoxObject(glm::vec2(400, 100), glm::vec2(250, 250), ResourceManager::GetTexture("box"));
    FootPrint = new GameObject(glm::vec2(500, 200), glm::vec2(50, 50), ResourceManager::GetTexture("foot-print"));
    // load FoodObjects in array
    for (int i = 0; i < 10; i++) {
        //420,330 max
        //400,230 min rand spawn pos
        int ranX = rand() % 20 + 400;
        int ranY = rand() % 100 + 230;
        auto *fo = new FoodObject(glm::vec2(ranX, ranY), glm::vec2(50, 50), ResourceManager::GetTexture("food"), 45.0f);
        FoodArray.push_back(fo);
    }
    // load CatObjects in array
    for (int i = 0; i < 5; i++) {
        int ranY = rand() % 50 + 600; //rand spawn pos
        auto *co = new CatObject(glm::vec2(800, ranY), glm::vec2(200, 200),
                                 ResourceManager::GetTexture("cat" + (std::to_string(i + 1))));
        CatArray.push_back(co);
    }
    // load ButtonObjects in array
    int y = 100;
    for (int i = 0; i < 3; i++) {
        auto *bo = new ButtonObject(glm::vec2(810, y), glm::vec2(380, 200), ResourceManager::GetTexture("border"), i);
        BArray.push_back(bo);
        y += 210;
    }
}

void Game::Update(float dt) {
    //std::cout << Money << "\n";

    Box->Shake();

    //food moving
    for (auto &f: FoodArray) {
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
    for (auto &c: CatArray) {
        if (c->Position.x > 0 - (c->Size.x)) {
            c->Move(dt);
        } else {
            c->Destroyed = true;
            c->Move(dt);
        }
    }

    // Auto clicker
    if (Box->ACExist) {
        if ((int) glfwGetTime() % Box->ACSpeed) {
            FootPrint->Destroyed = true;
        } else {
            FootPrint->Destroyed = false;

            if (dtTracker >= 1) {
                int randX = rand() % 100 + 450;
                int randY = rand() % 200 + 100;
                FootPrint->Position = glm::vec2(randX, randY);
                dtTracker = 0;
                Money += Box->FoodValue;
            } else {
                dtTracker += dt;
            }
        }
    } else {
        FootPrint->Destroyed = true;
    }

    // buttons availability update
    if (Money >= Box->CFoodDrop) {
        BArray[0]->Available = true;
    } else {
        BArray[0]->Available = false;
    }
    if (Money >= Box->CFoodValue) {
        BArray[1]->Available = true;
    } else {
        BArray[1]->Available = false;
    }
    if (Money >= Box->CAC) {
        BArray[2]->Available = true;
    } else {
        BArray[2]->Available = false;
    }

}

void Game::ProcessInput(float dt, GLFWwindow *window) {
    if (this->State == GAME_ACTIVE) {
        //https://community.khronos.org/t/how-do-i-detect-only-1-mouse-press-with-glfw/75132/2
        int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS && Box->CheckCollisionMouse(mouseX, mouseY)) {

            if (!(Box->Shaking)) Box->setShaking(true);

            for (int i = 0; i < Box->FoodDrop; i++) {
                if (IterFood <= FoodArray.size() - 1) {
                    FoodArray[IterFood]->Destroyed = false;
                    Money += Box->FoodValue;
                    if (IterFood != 9) {
                        IterFood++;
                    } else {
                        IterFood = 0;
                    }
                }
            }

        } else if (newState == GLFW_RELEASE && oldState == GLFW_PRESS &&
                   BArray[0]->CheckCollisionMouse(mouseX, mouseY)) {
            Money -= Box->Upgrade(0, Money); // Food drop rate
        } else if (newState == GLFW_RELEASE && oldState == GLFW_PRESS &&
                   BArray[1]->CheckCollisionMouse(mouseX, mouseY)) {
            Money -= Box->Upgrade(1, Money); // Food value
        } else if (newState == GLFW_RELEASE && oldState == GLFW_PRESS &&
                   BArray[2]->CheckCollisionMouse(mouseX, mouseY)) {
            Money -= Box->Upgrade(2, Money); // Auto clicker existence & speed
        }

        oldState = newState;
    }
}

void Game::Render() {
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

        if (!FootPrint->Destroyed) {
            FootPrint->Draw(*Renderer);
        }
        glFinish();

        for (auto &f: FoodArray) {
            if (!f->Destroyed) f->Draw(*Renderer);
        }
        glFinish();

        // draw food bowl "in front of" food
        if (Money <= 10) {
            myTexture = ResourceManager::GetTexture("bowl-front1");
            Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f,
                                 glm::vec3(1.0f, 1.0f, 1.0f));
        } else if (Money <= 50) {
            myTexture = ResourceManager::GetTexture("bowl-front2");
            Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f,
                                 glm::vec3(1.0f, 1.0f, 1.0f));
        } else if (Money <= 100) {
            myTexture = ResourceManager::GetTexture("bowl-front3");
            Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f,
                                 glm::vec3(1.0f, 1.0f, 1.0f));
        } else {
            myTexture = ResourceManager::GetTexture("bowl-front4");
            Renderer->DrawSprite(myTexture, glm::vec2(100, 300), glm::vec2(450, 450), 0.0f,
                                 glm::vec3(1.0f, 1.0f, 1.0f));
        }

        // draw cats
        int i = 1;
        for (auto &c: CatArray) {
            if (c->Position.x >= 400) {
                c->Sprite = ResourceManager::GetTexture("cat" + (std::to_string(i)));
                if (!c->Destroyed) c->Draw(*Renderer);
            } else if (c->Position.x < 400) {
                c->Sprite = ResourceManager::GetTexture("cath" + (std::to_string(i)));
                if (!c->Destroyed) c->Draw(*Renderer);
            }
            i++;
        }
        glFinish();

        // draw quad to hide outside spawning cats
        myTexture = ResourceManager::GetTexture("quad");
        Renderer->DrawSprite(myTexture, glm::vec2(800, 0), glm::vec2(400, 900), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        // draw buttons
        myTexture = ResourceManager::GetTexture("button");
        Renderer->DrawSprite(myTexture, glm::vec2(810, 100), glm::vec2(380, 200), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("button");
        Renderer->DrawSprite(myTexture, glm::vec2(810, 310), glm::vec2(380, 200), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("button");
        Renderer->DrawSprite(myTexture, glm::vec2(810, 520), glm::vec2(380, 200), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("dropU");
        Renderer->DrawSprite(myTexture, glm::vec2(920, 135), glm::vec2(140, 140), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("valueU");
        Renderer->DrawSprite(myTexture, glm::vec2(920, 345), glm::vec2(140, 140), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        myTexture = ResourceManager::GetTexture("acU");
        Renderer->DrawSprite(myTexture, glm::vec2(920, 555), glm::vec2(140, 140), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        for (auto &b: BArray) {
            if (b->Available) {
                b->Color = glm::vec3(1.0f, 1.0f, 1.0f);
                b->Draw(*Renderer);
            } else {
                b->Color = glm::vec3(1.0f, 0.5f, 0.5f);
                b->Draw(*Renderer);
            }
        }

    }
}

void Game::CatSpawn() {
    if (Money < 10) {
        CatCount = 0;
    }
    if (Money >= 30) {
        CatCount = 1;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 70) {
        CatCount = 2;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 120) {
        CatCount = 3;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 200) {
        CatCount = 4;
        CatArray[CatCount - 1]->Destroyed = false;
    }
    if (Money >= 300) {
        CatCount = 5;
        CatArray[CatCount - 1]->Destroyed = false;
    }
}

void Game::loadAllTextures() {
    ResourceManager::LoadTexture("img/background.png", true, "bg");
    ResourceManager::LoadTexture("img/box.png", true, "box");
    ResourceManager::LoadTexture("img/bowl-front.png", true, "bowl-front1");
    ResourceManager::LoadTexture("img/bowl1.png", true, "bowl-front2");
    ResourceManager::LoadTexture("img/bowl2.png", true, "bowl-front3");
    ResourceManager::LoadTexture("img/bowl3.png", true, "bowl-front4");
    ResourceManager::LoadTexture("img/bowl-back.png", true, "bowl-back");
    ResourceManager::LoadTexture("img/food.png", true, "food");
    ResourceManager::LoadTexture("img/cat1.png", true, "cat1");
    ResourceManager::LoadTexture("img/cat2.png", true, "cat2");
    ResourceManager::LoadTexture("img/cat3.png", true, "cat3");
    ResourceManager::LoadTexture("img/cat4.png", true, "cat4");
    ResourceManager::LoadTexture("img/cat5.png", true, "cat5");
    ResourceManager::LoadTexture("img/cat1h.png", true, "cath1");
    ResourceManager::LoadTexture("img/cat2h.png", true, "cath2");
    ResourceManager::LoadTexture("img/cat3h.png", true, "cath3");
    ResourceManager::LoadTexture("img/cat4h.png", true, "cath4");
    ResourceManager::LoadTexture("img/cat5h.png", true, "cath5");
    ResourceManager::LoadTexture("img/button.png", true, "button");
    ResourceManager::LoadTexture("img/dropUp.png", true, "dropU");
    ResourceManager::LoadTexture("img/valueU.png", true, "valueU");
    ResourceManager::LoadTexture("img/acU.png", true, "acU");
    ResourceManager::LoadTexture("img/quad.png", true, "quad");
    ResourceManager::LoadTexture("img/foot-print.png", true, "foot-print");
    ResourceManager::LoadTexture("img/border.png", true, "border");
}
