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
    float collision_radius;
    float rotation;
    float speed;
};

char GAME_TITLE_MESSAGE[] = "click to play";
char GAME_FAIL_MESSAGE[] = "died game over :(";
char GAME_SUCCESS_MESSAGE[] = "survived :)";

struct SpaceCraft
{
    Vector2             position;
    Texture2D           texture[2];
    enum State          state;
    struct Missile      missile;
    float               collision_radius;
    float               rotation;
    float               fuel;
    float               power;
    bool                is_fire;
};


void HandleInputs(struct SpaceCraft* const);
void DrawSpacecraft(Texture2D* const,Vector2,float);
void DrawMissile(Texture2D* const, Vector2 , float);

int main(void)
{
    
    // Initialization
    //--------------------------------------------------------------------------------------
    //debugging
    //char LoggingBuffer[50];
    //setup window
    const int SCREEN_WIDTH  =       1600;
    const int SCREEN_HEIGHT =       1200;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "a shooting game created with Raylib");
    
    //setup the game environment
    time_t t;
    srand((unsigned int) time (&t));
    const float GAME_TIME   =       60.0f;
    const float UFO_FIRE_INTERVAL  = 0.25;
    float last_ufo_fire = 0.0f;
    bool is_ufo_fire = false;
    bool is_survive = false;
    float fire_angle = 0;
    float TimeRemaining = GAME_TIME;
    enum GameScreen currentScreen = LOGO;
    char RemainTimeBuffer[15];
    
    Image spacecraft_image = LoadImage("resources/spacecraft_a.png");
    Image spacecraft_fire = LoadImage("resources/spacecraft_fire.png");
    //setup player spacecraft
    struct SpaceCraft player_ship = {
         .position.x = 250,
         .position.y = 250,
         .state = ALIVE,
         .rotation = 0.0f,
         .fuel = 100.0f,
         .power = 0.0f,
         .collision_radius = 25.0f,
         .texture[0] = LoadTextureFromImage(spacecraft_image),
         .texture[1] = LoadTextureFromImage(spacecraft_fire),
    };
    
    //setup ufo spacecraft
    float ufo_rot = ((float)rand()/RAND_MAX * (float)120.0f + 220.0f);
    Image ufo_image = LoadImage("resources/spacecraft_b.png");
    struct SpaceCraft ufo_a = { 
    .position.x = SCREEN_WIDTH - 250, 
    .position.y = 250, 
    .state = ALIVE, 
    .rotation = 0.0f,
    .fuel = 100.0f,
    .collision_radius = 25.0f,
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
    .power = 105.0f,
    .collision_radius = 25.0f,
    };
    UnloadImage(ufo_image);
    
    
    //setup missile
    Image img_missile = LoadImage("resources/spacecraft_bullet.png");
    player_ship.missile.texture = LoadTextureFromImage(img_missile);
    player_ship.missile.speed = 250;
    player_ship.missile.collision_radius = 10.0f;
    
    ufo_a.missile.texture = LoadTextureFromImage(img_missile);
    ufo_b.missile.texture = LoadTextureFromImage(img_missile);
    ufo_a.missile.speed = 200;
    ufo_a.missile.collision_radius = 10.0f;
    ufo_b.missile.speed = 200;
    ufo_b.missile.collision_radius = 10.0f;
    UnloadImage(img_missile);
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Image img_logo = LoadImage("resources/raylib_logo.png");
    Texture2D texture_logo = LoadTextureFromImage(img_logo);
    UnloadImage(img_logo);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        HandleInputs(&player_ship);
        bool isPlayerUsingPower = false;
        //scene update
        switch(currentScreen)
        {
        case LOGO:
        {
            last_ufo_fire = 0.0f;
            is_ufo_fire = false;
            is_survive = false;
            fire_angle = 0;
            TimeRemaining = GAME_TIME;
            player_ship.position.x = 250;
            player_ship.position.y = 250;
            player_ship.state = ALIVE;
            player_ship.rotation = 0.0f;
            player_ship.fuel = 100.0f;
            player_ship.power = 0.0f;
            ufo_a.position.x = SCREEN_WIDTH - 250; 
            ufo_a.position.y = 250; 
            ufo_a.state = ALIVE;
            ufo_b.position.x = SCREEN_WIDTH - 250;
            ufo_b.position.y = 500;
            ufo_b.state = ALIVE;
            currentScreen = TITLE;
        }
        break;
        case TITLE:
        if(IsMouseButtonReleased(0))
        {
            currentScreen = GAMEPLAY;
        }
        break;
        case GAMEPLAY:
        {
            //update player_ship positions
            float radian = (-45.0f + player_ship.rotation) * 3.141592f / 180.0f;
            player_ship.position.x += (player_ship.power * (cos(radian) + sin(radian)) * GetFrameTime());
            if(player_ship.position.x > SCREEN_WIDTH)
            {
                player_ship.position.x  = 0.0f;
            }
            else if( player_ship.position.x < 0.0f)
            {
                player_ship.position.x = SCREEN_WIDTH;
            }
            player_ship.position.y += (player_ship.power * (sin(radian) - cos (radian)) * GetFrameTime());
            if(player_ship.position.y > SCREEN_HEIGHT)
            {
                player_ship.position.y = 0.0f;
            }
            else if(player_ship.position.y < 0.0f)
            {
                player_ship.position.y = SCREEN_HEIGHT;
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
            else if(ufo_b.position.y >= 1150.0f)
            {
                ufo_rot = ((float)rand()/RAND_MAX * (float)45.0f - 60.0f);
            }
            
            float ufo_radian = (-45.0f + ufo_rot) * (3.141592f / 180.0f);
            if(ufo_a.state == ALIVE)
            {
                ufo_a.position.x += (ufo_a.power  * (cos(ufo_radian) + sin(ufo_radian)) * GetFrameTime());
                ufo_a.position.y += (ufo_a.power * (sin(ufo_radian) - cos(ufo_radian)) * GetFrameTime());                    
            }
            if(ufo_b.state == ALIVE)
            {
                ufo_b.position.x += (ufo_b.power  * (cos(ufo_radian) + sin(ufo_radian)) * GetFrameTime());
                ufo_b.position.y += (ufo_b.power * (sin(ufo_radian) - cos(ufo_radian)) * GetFrameTime());                    
            }

            
            //update player missile position
            if(player_ship.is_fire)
            {
                float missile_radian = (-45.0f + player_ship.missile.rotation) * 3.141592f / 180.0f;
                player_ship.missile.position.x += (player_ship.missile.speed * (cos(missile_radian) + sin(missile_radian)) * GetFrameTime());
                player_ship.missile.position.y += (player_ship.missile.speed * (sin(missile_radian) - cos (missile_radian)) * GetFrameTime());
                
                float x_delta = fabs(player_ship.missile.position.x  - player_ship.position.x);
                float y_delta = fabs(player_ship.missile.position.y - player_ship.position.y);
                
                if(ufo_a.state == ALIVE && CheckCollisionCircles(ufo_a.position,ufo_a.collision_radius,player_ship.missile.position,player_ship.missile.collision_radius))
                {
                    ufo_a.state = DEAD;
                }
                if(ufo_b.state == ALIVE && CheckCollisionCircles(ufo_b.position,ufo_b.collision_radius,player_ship.missile.position,player_ship.missile.collision_radius))
                {
                    ufo_b.state = DEAD;
                }            
                if(ufo_a.state == DEAD && ufo_b.state == DEAD)
                {
                    currentScreen = ENDING;
                    is_survive = true;
                }
                if( x_delta + y_delta  > 300 )
                {
                    player_ship.is_fire = false;
                }
            }
            
            
            //update ufos missile
            double current_time = GetTime();
        
            if(current_time >= last_ufo_fire + UFO_FIRE_INTERVAL)
            {
                ufo_a.missile.position = ufo_a.position;
                ufo_b.missile.position = ufo_b.position;
                is_ufo_fire = true;
                float x = player_ship.position.x - ufo_a.position.x;
                float y = player_ship.position.y - ufo_a.position.y;
                fire_angle = atan2(y,x);
                fire_angle += (3.141592f / 4.0f);
            }
            
            if(is_ufo_fire)
            {
                last_ufo_fire = current_time;
                //fire
                if(ufo_a.state == ALIVE)
                {
                    ufo_a.missile.position.x += (ufo_a.missile.speed * (cos(fire_angle) + sin(fire_angle)) * GetFrameTime());
                    ufo_a.missile.position.y += (ufo_a.missile.speed * (sin(fire_angle) - cos(fire_angle)) * GetFrameTime());
                    if(CheckCollisionCircles(player_ship.position,player_ship.collision_radius,ufo_a.missile.position,ufo_a.missile.collision_radius))
                    {
                        currentScreen = ENDING;
                    }
                }
                if(ufo_b.state == ALIVE)
                {
                    ufo_b.missile.position.x += (ufo_b.missile.speed * (cos(fire_angle) + sin(fire_angle)) * GetFrameTime());
                    ufo_b.missile.position.y += (ufo_b.missile.speed * (sin(fire_angle) - cos(fire_angle)) * GetFrameTime());
                    if(CheckCollisionCircles(player_ship.position,player_ship.collision_radius,ufo_b.missile.position,ufo_b.missile.collision_radius))
                    {
                        currentScreen = ENDING;
                    }
                }                 
                float x_delta = fabs(ufo_a.missile.position.x - ufo_a.position.x);
                float y_delta = fabs(ufo_a.missile.position.y - ufo_a.position.y);
                if( x_delta + y_delta  > 1000 )
                {
                    is_ufo_fire = false;
                }
                
            }
        
            TimeRemaining -= GetFrameTime();
            sprintf(RemainTimeBuffer,"Remaining : %d",(int)TimeRemaining);
            
            isPlayerUsingPower = IsKeyDown(KEY_UP);
        }
        break;
        case ENDING:
        if(IsMouseButtonReleased(0))
        {
            currentScreen = LOGO;
        }
        break;
        }
        
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            
            switch(currentScreen)
            {
            case LOGO:
            DrawText("Loading...",300,300,30,YELLOW);
            case TITLE:
            DrawText(GAME_TITLE_MESSAGE,SCREEN_WIDTH / 2 - 300,SCREEN_HEIGHT / 2,50,YELLOW);
            DrawTexturePro(texture_logo,
            (Rectangle){0,0, texture_logo.width,texture_logo.height},
            (Rectangle){1200,1000,texture_logo.width / 2,texture_logo.height / 2},
            (Vector2){texture_logo.width/4, texture_logo.height/4 },
            0.0f,
            WHITE);
            break;
            case GAMEPLAY:
            {
                //draw player spacecraft
                if(player_ship.state == ALIVE)
                {
                    DrawSpacecraft(&player_ship.texture[0],player_ship.position, player_ship.rotation);
                    if(isPlayerUsingPower)
                    {
                        DrawSpacecraft(&player_ship.texture[1],player_ship.position,player_ship.rotation);
                    }
                    if(player_ship.is_fire)
                    {
                        DrawMissile(&player_ship.missile.texture,player_ship.missile.position,player_ship.missile.rotation);
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
                
                if(is_ufo_fire)
                {
                    if(ufo_a.state ==ALIVE)
                    {
                        DrawMissile(&ufo_a.missile.texture,ufo_a.missile.position,ufo_a.missile.rotation);
                    }
                    if(ufo_b.state == ALIVE)
                    {
                        DrawMissile(&ufo_b.missile.texture,ufo_b.missile.position,ufo_b.missile.rotation);
                    }
                    
                }
               
                //draw UIs
                DrawText(RemainTimeBuffer,25,25,30,GREEN);
            }
            break;
            case ENDING:
            if(is_survive)
            {
                DrawText(GAME_SUCCESS_MESSAGE,SCREEN_WIDTH / 2 - 200,SCREEN_HEIGHT / 2,50,YELLOW);
            }
            else
            {
                DrawText(GAME_FAIL_MESSAGE,SCREEN_WIDTH / 2 - 200,SCREEN_HEIGHT / 2,50,YELLOW);
            }
            
            break;
            }
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(player_ship.texture[0]);
    UnloadTexture(player_ship.texture[1]);
    UnloadTexture(player_ship.missile.texture);
    UnloadTexture(ufo_a.texture[0]);
    UnloadTexture(ufo_b.texture[0]);
    UnloadTexture(ufo_a.missile.texture);
    UnloadTexture(ufo_b.missile.texture);
    UnloadTexture(texture_logo);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void DrawSpacecraft(Texture2D* const texture,Vector2 position, float rotation)
{
    //DrawCircle(position.x,position.y,25.0f,RED);
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
    //DrawCircle(position.x,position.y,15.0f,RED);
    DrawTexturePro(
        *texture,
        (Rectangle){0,0,texture->width,texture->height},
        (Rectangle){position.x,position.y,texture->width * 2,texture->height * 2},
        (Vector2){texture->width,texture->height},
        rotation,
        WHITE);
}

void HandleInputs(struct SpaceCraft * const spacecraft)
{
    if(IsKeyDown(KEY_RIGHT))
    {
        spacecraft->rotation += 1.0f;
    }
    if(IsKeyDown(KEY_LEFT))
    {
        spacecraft->rotation -= 1.0f;
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
        spacecraft->missile.position = spacecraft->position;
        spacecraft->missile.rotation = spacecraft->rotation;
        
    }
}