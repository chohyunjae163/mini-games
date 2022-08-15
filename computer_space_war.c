/*******************************************************************************************
* Raylib - computerspacewargame
* Author - Hyun Jae Cho
* Since - 2022-08-02
********************************************************************************************/

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

enum GameScreen{ LOGO = 0, TITLE, GAMEPLAY, ENDING };
enum State{ DEAD = 0, ALIVE, INVINCIBLE };
struct Missile
{
    Texture2D texture;
    Vector2 position;
    float rotation;
    float speed;
};

struct SpaceCraft
{
    Vector2             position;
    Texture2D           texture[2];
    enum State          state;
    struct Missile      missile;
    float               rotation;
    float               fuel;
    float               power;
    bool                is_fire;
};


void HandleInputs(struct SpaceCraft* const);
void DrawSpacecraft(Texture2D* const,Vector2,float);
void DrawMissile(Texture2D* const, Vector2 , float);
float IncreaseRotation(float);
float DecreaseRotation(float);
void fire(struct Missile* const,Vector2, float);

int main(void)
{
    time_t t;
    srand((unsigned int) time (&t));
    // Initialization
    //--------------------------------------------------------------------------------------
    //setup window
    const int SCREEN_WIDTH  =       1600;
    const int SCREEN_HEIGHT =       1200;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "a shooting game created with Raylib");
    
    //setup the game environment
    const float GAME_TIME   =       60.0f;
    enum GameScreen currentScreen = LOGO;
    float TimeRemaining = GAME_TIME;
    int score = 0;
    char RemainTimeBuffer[15];
    char ScoreBuffer[18];
    
    //Image spacecraft_image = LoadImage("resources/spacecraft_a.png");
    //Image spacecraft_fire = LoadImage("resources/spacecraft_fire.png");
    //setup player spacecraft
    // struct SpaceCraft PlayerShip = {
         // .position.x = 250,
         // .position.y = 250,
         // .state = ALIVE,
         // .rotation = 0.0f,
         // .fuel = 100.0f,
         // .texture[0] = LoadTextureFromImage(spacecraft_image),
         // .texture[1] = LoadTextureFromImage(spacecraft_fire),
    // };
    struct SpaceCraft PlayerShip;
    PlayerShip.position.x = 250;
    PlayerShip.position.y = 250;
    PlayerShip.state = ALIVE;
    PlayerShip.rotation  = 0.0f;
    PlayerShip.fuel = 100.0f;
    Image spacecraft_image = LoadImage("resources/spacecraft_a.png");
    PlayerShip.texture[0] = LoadTextureFromImage(spacecraft_image);
    Image spacecraft_fire = LoadImage("resources/spacecraft_fire.png");
    PlayerShip.texture[1] = LoadTextureFromImage(spacecraft_fire);
    UnloadImage(spacecraft_image);
    UnloadImage(spacecraft_fire);
    
    //setup ufo spacecraft
    float ufo_rot = ((float)rand()/RAND_MAX * (float)120.0f + 220.0f);
    Image ufo_image = LoadImage("resources/spacecraft_b.png");
    struct SpaceCraft ufo_a = { 
    .position.x = SCREEN_WIDTH - 250, 
    .position.y = 250, 
    .state = ALIVE, 
    .rotation = 0.0f,
    .fuel = 100.0f,
    .texture[0] = LoadTextureFromImage(ufo_image),
    .power = 105.0f
    };
    
    struct SpaceCraft ufo_b = { 
    .position.x = SCREEN_WIDTH - 250, 
    .position.y = 500, 
    .state = ALIVE, 
    .rotation = 0.0f,
    .fuel = 100.0f,
    .texture[0] = LoadTextureFromImage(ufo_image),
    .power = 105.0f
    };
    UnloadImage(ufo_image);
    
    
    //setup missile
    Image img_missile = LoadImage("resources/spacecraft_bullet.png");
    PlayerShip.missile.texture = LoadTextureFromImage(img_missile);
    PlayerShip.missile.speed = 250;
    UnloadImage(img_missile);
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        HandleInputs(&PlayerShip);
        //if pressed ESC, end the game
        if(IsKeyDown(KEY_ESCAPE))
        {
            break;
        }    
        //scene update
        switch(currentScreen)
        {
            case LOGO:
            break;
            case TITLE:
            break;
            case GAMEPLAY:
            break;
            case ENDING:
            break;
        }
        
        //update playership positions
        float radian = (-45.0f + PlayerShip.rotation) * 3.141592f / 180.0f;
        PlayerShip.position.x += (PlayerShip.power * (cos(radian) + sin(radian)) * GetFrameTime());
        if(PlayerShip.position.x > SCREEN_WIDTH)
        {
            PlayerShip.position.x  = 0.0f;
        }
        else if( PlayerShip.position.x < 0.0f)
        {
            PlayerShip.position.x = SCREEN_WIDTH;
        }
        PlayerShip.position.y += (PlayerShip.power * (sin(radian) - cos (radian)) * GetFrameTime());
        if(PlayerShip.position.y > SCREEN_HEIGHT)
        {
            PlayerShip.position.y = 0.0f;
        }
        else if(PlayerShip.position.y < 0.0f)
        {
            PlayerShip.position.y = SCREEN_HEIGHT;
        }
        
        //do some random direction
        if(ufo_a.position.x > 1550.0f)
        {
            ufo_rot = ((float)rand()/RAND_MAX * (float)120.0f + 220.0f);
        }
        else if(ufo_a.position.x <= 50.0f)
        {
            ufo_rot = ((float)rand()/RAND_MAX * (float)120.0f + 30.0f);
        }
        if(ufo_a.position.y <= 50.0f)
        {
            ufo_rot = ((float)rand()/RAND_MAX * (float)120.0f + 120.0f);
        }
        else if(ufo_a.position.y >= 1150.0f)
        {
            ufo_rot = ((float)rand()/RAND_MAX * (float)45.0f - 60.0f);
        }
        
        float ufo_radian = (-45.0f + ufo_rot) * (3.141592f / 180.0f);
        ufo_a.position.x += (ufo_a.power  * (cos(ufo_radian) + sin(ufo_radian)) * GetFrameTime());
        ufo_a.position.y += (ufo_a.power * (sin(ufo_radian) - cos(ufo_radian)) * GetFrameTime());
        ufo_b.position.x += (ufo_b.power  * (cos(ufo_radian) + sin(ufo_radian)) * GetFrameTime());
        ufo_b.position.y += (ufo_b.power * (sin(ufo_radian) - cos(ufo_radian)) * GetFrameTime());
        
        //update player missile position
        if(PlayerShip.is_fire)
        {
            float missile_radian = (-45.0f + PlayerShip.missile.rotation) * 3.141592f / 180.0f;
            PlayerShip.missile.position.x += (PlayerShip.missile.speed * (cos(missile_radian) + sin(missile_radian)) * GetFrameTime());
            PlayerShip.missile.position.y += (PlayerShip.missile.speed * (sin(missile_radian) - cos (missile_radian)) * GetFrameTime());
            
            float x_delta = fabs(PlayerShip.missile.position.x  - PlayerShip.position.x);
            float y_delta = fabs(PlayerShip.missile.position.y - PlayerShip.position.y);
            if( x_delta + y_delta  > 300 )
            {
                PlayerShip.is_fire = false;
            }
        }

        TimeRemaining -= GetFrameTime();
        sprintf(RemainTimeBuffer,"Remaining : %d",(int)TimeRemaining);
        sprintf(ScoreBuffer,"Score:%.3f", PlayerShip.position.x);
        
        bool isPlayerUsingPower = IsKeyDown(KEY_UP);
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            
            //draw player spacecraft
            if(PlayerShip.state == ALIVE)
            {
                DrawSpacecraft(&PlayerShip.texture[0],PlayerShip.position, PlayerShip.rotation);
                if(isPlayerUsingPower)
                {
                    DrawSpacecraft(&PlayerShip.texture[1],PlayerShip.position,PlayerShip.rotation);
                }
                if(PlayerShip.is_fire)
                {
                    DrawMissile(&PlayerShip.missile.texture,PlayerShip.missile.position,PlayerShip.missile.rotation);
                }
            }
            
            //draw ufos!
            if(ufo_a.state == ALIVE)
            {
               DrawSpacecraft(&ufo_a.texture[0],ufo_a.position,0.0f);    
            }
            if(ufo_b.state == ALIVE)
            {
               DrawSpacecraft(&ufo_b.texture[0],ufo_b.position, 0.0f);
            }
            
            
            
            //draw UIs
            DrawText(RemainTimeBuffer,25,25,30,GREEN);
            DrawText(ScoreBuffer,25,75,30,GREEN);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(PlayerShip.texture[0]);
    UnloadTexture(PlayerShip.texture[1]);
    UnloadTexture(PlayerShip.missile.texture);
    UnloadTexture(ufo_a.texture[0]);
    UnloadTexture(ufo_b.texture[0]);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

float IncreaseRotation(float rotation)
{
    return rotation + 1.0f;
}


float DecreaseRotation(float rotation)
{
    return rotation - 1.0f;
}

void DrawSpacecraft(Texture2D* const texture,Vector2 position, float rotation)
{
    DrawTexturePro(
        *texture,
        (Rectangle){0,0,texture->width,texture->height},
        (Rectangle){position.x,position.y,texture->width * 2,texture->height * 2},
        (Vector2){texture->width,texture->height},
        rotation,
        WHITE);
}

void DrawMissile(Texture2D* const texture,Vector2 position, float rotation)
{
    DrawTexturePro(
        *texture,
        (Rectangle){0,0,texture->width,texture->height},
        (Rectangle){position.x,position.y,texture->width,texture->height},
        (Vector2){texture->width / 2,texture->height / 2},
        rotation,
        WHITE);
}

void HandleInputs(struct SpaceCraft * const spacecraft)
{
    if(IsKeyDown(KEY_RIGHT))
    {
        spacecraft->rotation = IncreaseRotation(spacecraft->rotation);
    }
    if(IsKeyDown(KEY_LEFT))
    {
        spacecraft->rotation = DecreaseRotation(spacecraft->rotation);
    }
    if(IsKeyDown(KEY_UP))
    {
       spacecraft->fuel -= 1;
       if(spacecraft->fuel < 1)
       {
           spacecraft->fuel = 0;
       }
       else
       {
           spacecraft->power += 1;
       } 
    }
    
    if(IsKeyPressed(KEY_SPACE))
    {
        if(spacecraft->is_fire)
        {
            return;
        }
        spacecraft->is_fire = true;
        fire(&spacecraft->missile,spacecraft->position, spacecraft->rotation);
    }
}


void fire(struct Missile* const missile, Vector2 position, float rotation)
{
    missile->position = position;
    missile->rotation = rotation;
}
