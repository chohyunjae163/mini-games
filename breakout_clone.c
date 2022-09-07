#include "raylib.h" 
#include <math.h>

#define ROW 8
#define COLUMN 14
#define BRICKS_NUM ROW * COLUMN // eight rows and fourteen columns
#define BRICK_COLOR(row) row < 2 ? RED : row < 4 ? ORANGE : row < 6 ? GREEN : YELLOW
#define WALL_THICKNESS 25
#define LEFT_WALL 50
#define RIGHT_WALL 1550
#define WALL_PADDING 10


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 1200;
    
    InitWindow(screenWidth, screenHeight, "breakout with raylib.");
        
    // TODO: Load resources / Initialize variables at this point
    InitAudioDevice();      // Initialize audio device
    SetTargetFPS(60);
    
    const int   brickWidth = 90;
    const int   brickHeight = 20;
    const int   brickPadding = 10;
    const int   xPadding = 100;
    const int   yPadding = 200;
    const int   rightWallBound = screenWidth - WALL_THICKNESS - WALL_PADDING;
    const int   leftWallBound = WALL_PADDING + WALL_THICKNESS;   
    int         playerWidth = 100;
    int         playerHeight = 20;
    bool        IsBallDetached = false;
    bool        destroyed[BRICKS_NUM] = {0};
    Vector2     brickPositions[BRICKS_NUM] = {0};
    for(int i = 0; i < BRICKS_NUM;++i)
    {
        brickPositions[i].x = xPadding + (brickWidth + brickPadding) * (i % COLUMN);
        brickPositions[i].y = yPadding + (brickHeight + brickPadding) * (i / COLUMN);
    }

    Vector2 playerPosition = { .x = screenWidth / 2, .y = screenHeight - 100 };
    Vector2 ballStartPos = { .x = playerPosition.x + playerWidth / 2, .y = playerPosition.y - playerHeight / 2  };
    Vector2 ballPos = ballStartPos;
    Vector2 ballDir = {.x = 0.0f, .y = -1.0f };
    float   ballSpeed = 5.0f;
    const float playerSpeed = 5.0f;
    
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update variables / Implement example logic at this point
        //----------------------------------------------------------------------------------
        // process inputs
        if(IsBallDetached)
        {
            if (IsKeyDown(KEY_RIGHT))
            {
                playerPosition.x += playerSpeed;
            }
            if (IsKeyDown(KEY_LEFT))
            {
                playerPosition.x -= playerSpeed;     
            }

            if(playerPosition.x < leftWallBound)
            {
                playerPosition.x = leftWallBound;
            }
            if(playerPosition.x > rightWallBound - playerWidth)
            {
                playerPosition.x = rightWallBound - playerWidth;
            }
        }
        //the ball is attached to the player when the game starts.
        //detach the ball in order to start the game.
        if (IsKeyDown(KEY_SPACE))
        {
            IsBallDetached = true;
        }
        
        if(IsBallDetached)
        {
            ballPos.x += (ballDir.x * ballSpeed);
            ballPos.y += (ballDir.y * ballSpeed);
        }
        
        //brick and ball collision check.destory when collided.
        for( int i = 0 ;i < BRICKS_NUM; ++i )
        {
            if(destroyed[i])
            {
                continue;
            }
            Rectangle brick = { 
                .x = brickPositions[i].x,
                .y = brickPositions[i].y,
                .width = brickWidth,
                .height = brickHeight,
            };
            
            if(CheckCollisionPointRec(ballPos, brick))
            {
                destroyed[i] = true;
                //assume the normal vector of all the bricks is (0,1)
                Vector2 brickNormal = { .x = 0.0f, .y = 1.0f};
                // reflection vector : R = P +  2n(-P·n)
                //maybe i dont need all this math....i can just flip x or y value of a vector..
                float dot = -ballDir.x * brickNormal.x + -ballDir.y * brickNormal.y;
                Vector2 proj = {.x = brickNormal.x * dot, .y = brickNormal.y * dot};
                Vector2 reflect = {.x = ballDir.x +  2 * proj.x , .y = ballDir.y + 2 * proj.y };
                float length = sqrt(reflect.x * reflect.x + reflect.y * reflect.y);
                if(length > 0.0f)
                {
                    reflect.x /= length;
                    reflect.y /= length;
                }
                ballDir = reflect;
            }
            
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            // TODO: Draw everything that requires to be drawn at this point:
            //draw walls
            DrawRectangle(
                WALL_PADDING,
                WALL_PADDING,
                screenWidth - (2 * WALL_PADDING),
                WALL_THICKNESS,
                LIGHTGRAY);
            DrawRectangle(
                WALL_PADDING,
                WALL_PADDING,
                WALL_THICKNESS,
                screenHeight - (2 * WALL_PADDING),
                LIGHTGRAY);
            DrawRectangle(
                screenWidth - WALL_PADDING - WALL_THICKNESS,
                WALL_PADDING,
                WALL_THICKNESS,
                screenHeight - (2 * WALL_PADDING),
                LIGHTGRAY);
            
            //draw bricks with five colours
            //red -> orange -> green -> yellow. two rows each.
            for(int index = 0; index < BRICKS_NUM; ++index)
            {
                if(destroyed[index])
                {
                    continue;
                }
                DrawRectangle(
                    brickPositions[index].x,
                    brickPositions[index].y, 
                    brickWidth, 
                    brickHeight, 
                    BRICK_COLOR(index / COLUMN ));
            }
            
            //draw a player rectangle
            DrawRectangle(playerPosition.x,playerPosition.y,playerWidth,playerHeight,BEIGE);
            //draw a ball
            DrawCircle(ballPos.x,ballPos.y,10,PURPLE);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded resources at this point
    StopSoundMulti();       // We must stop the buffer pool before unloading
    CloseAudioDevice();     // Close audio device
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
