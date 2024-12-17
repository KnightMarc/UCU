#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color {38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Ligh_Green = Color {129, 204, 184, 255};
Color Yellow = Color {243, 213, 91, 255};

int playerScore = 0;
int cpuScore = 0;


class Ball{
    public:
    //position of the ball
    float x, y;
    //Speed of the ball
    int speedx, speedy;
    //create a radius do the ball is flexible
    int radius;

    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }

    //so the ball is moving
    //the ball moves at the speed of 7 px/sec
    void Update(){
        x += speedx;
        y += speedy;

        //check of the ball is on the top and bottom of the screen
        if(y + radius >= GetScreenHeight() || y - radius <=0 ){
            speedy *= -1;
        }
        if(x + radius >= GetScreenWidth()){//Player win a score
            playerScore++;
            ResetBall();
        }
        
        if(x - radius <= 0 ){//Cpu will win
            cpuScore++;
            ResetBall();
        }
    }

    //new method to reset the ball
    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] ={-1, 1};
        speedx *= speed_choices[GetRandomValue(0,1)];
        speedy *= speed_choices[GetRandomValue(0,1)];


    }

};

//for control
class Paddle{
    protected:
    //create a protected method so other class can inheret it not just copy paste code
    void LimitMovement(){
         //paddle wont go outside the frame
        if(y <= 0){
            y = 0 ;
        }
         if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }

    public:
    float x,y;
    float width, height;
    int speed;

    //method responsible for drawing the paddle on the screen
    void Draw(){
        //Syntax(Position x, Position y, Width of the shape, Height of the shape, Color)
       // DrawRectangle(x , y, width, height, WHITE);
       //DrawRectangleRounded(rec, roundness, segment, color);
       DrawRectangleRounded(Rectangle{x,y, width, height}, 0.8, 0, WHITE);
    }

    //this is where the paddle can move by the player
    void Update(){
        if(IsKeyDown(KEY_W)){
            //move up the screen
            y = y - speed;
        }
        if(IsKeyDown(KEY_S)){
            //move down the screen
            y = y + speed;
        }
        LimitMovement();

       

    }
};

class CPUPaddle: public Paddle{
    public:
    void Update(int ball_y){
        if(IsKeyDown(KEY_UP)){
            //move up the screen
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN)){
            //move down the screen
            y = y + speed;
        }

        LimitMovement();
        
    }
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main() {
    const int screenwidth = 1280;
    const int screenheight = 800;
    InitWindow(screenwidth, screenheight, "My Pong Game");
    SetTargetFPS(60);

    bool isPaused = false; // Tracks the pause state

    // Initialize the ball
    ball.radius = 20;
    ball.x = screenwidth / 2;
    ball.y = screenheight / 2;
    ball.speedx = 7;
    ball.speedy = 7;

    // Initialize the player paddle
    player.width = 25;
    player.height = 120;
    player.x = 10;
    player.y = screenheight / 2 - player.height / 2;
    player.speed = 6;

    // Initialize the CPU paddle
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = screenwidth - cpu.width - 10;
    cpu.y = screenheight / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false) {
        // Toggle pause state when P is pressed
        if (IsKeyPressed(KEY_P)) {
            isPaused = !isPaused;
        }

        if (!isPaused) {
            // Update positions only if the game is not paused
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Check for collisions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                        Rectangle{player.x, player.y, player.width, player.height})) {
                ball.speedx *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                        Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speedx *= -1;
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(Dark_Green);

        // Background
        DrawRectangle(screenwidth / 2, 0, screenwidth / 2, screenheight, Green);
        DrawCircle(screenwidth / 2, screenheight / 2, 150, Ligh_Green);

        // Draw separator line
        DrawLine(screenwidth / 2, 0, screenwidth / 2, screenheight, WHITE);

        // Draw ball and paddles
        ball.Draw();
        player.Draw();
        cpu.Draw();

        // Draw scores
        DrawText(TextFormat("%i", playerScore), screenwidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpuScore), 3 * screenwidth / 4 - 20, 20, 80, WHITE);

        // Draw pause message if the game is paused
        if (isPaused) {
            DrawText("Game Paused", screenwidth / 2 - MeasureText("Game Paused", 50) / 2,
                     screenheight / 2 - 25, 50, WHITE);
            DrawText("Press P to Resume", screenwidth / 2 - MeasureText("Press P to Resume", 30) / 2,
                     screenheight / 2 + 25, 30, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
