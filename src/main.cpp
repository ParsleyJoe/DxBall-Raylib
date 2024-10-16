#include <iostream>
#include <vector>
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

std::vector<int> boxGrid = {
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1
};
int score = 0;

// Player Paddle so paddle ?
class paddle
{
public:
	float x, y;
	float width, height;
	float speed;

	void Draw()
	{
		DrawRectangle(x, y, width, height, RAYWHITE);
	}
	void Move()
	{
		if (IsKeyDown(KEY_LEFT))
		{
			x -= speed;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			x += speed;
		}
		LimitMovement();
	}
	void LimitMovement()
	{
		if (x <= 0)
		{
			x = 0;
		}
		if (x + width >= GetScreenWidth())
		{
			x = GetScreenWidth() - width;
		}
	}
};
paddle player;


class Ball
{
public:
	float x, y;
	int radius;
	float speedX;
	float speedY;
	float timeStopped;
	bool moving = true;
	
	
	void Draw()
	{
		DrawCircle(x, y, radius, RAYWHITE);
	}
	void Move()
	{
		if (moving)
		{
			x += speedX;
			y += speedY;
		}
		else
		{
			timeStopped += GetFrameTime();
			if (timeStopped >= 2)
			{
				timeStopped = 0;
				moving = true;
			}
		}
		
		LimitMovement();
	}
	void LimitMovement()
	{
		// Out of Bounds
		if (x >= GetScreenWidth() || x <= 0)
		{
			speedX *= -1;
		}
		if (y <= 0)
		{
			speedY *= -1;
		}
		if (y >= GetScreenHeight())
		{
			ResetBall();
			moving = false;
		}
		
		// Collision
		if (CheckCollisionCircleRec(Vector2{x, y}, radius, Rectangle{player.x, player.y, player.width, player.height}))
		{
			speedY *= -1;
		}
	}
	void ResetBall()
	{
		int xDir[2] = { 1, -1 };
		int yDir[2] = { 1, -1 };
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		speedX += GetRandomValue(xDir[0], xDir[1]);
		speedY += GetRandomValue(yDir[0], xDir[1]);
	}
};
Ball ball;

void DrawScore()
{
	float posX = 40;
	float posY = 20;

	DrawText(TextFormat("Score: %i", score), posX, posY, 40, RAYWHITE);
}

// Checks Collisions too
void DrawBoxes()
{

	int leftOffset = 50;
	int rightOffset = GetScreenWidth() - 50;
	float rectWidth = 90;
	float rectHeight = 15;
	float posX = leftOffset;
	float posY = 100;

	for (int i = 0; i < boxGrid.size(); i++)
	{
		if (boxGrid[i] == 1)
		{
			DrawRectangle(posX, posY, rectWidth, rectHeight, RAYWHITE);
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ posX, posY, rectWidth, rectHeight }) && boxGrid[i] == 1)
		{
			boxGrid[i] = 0;
			ball.speedY *= -1;
			score += 30; // Increase Score by 30 every time a box is destroyed
		}
		posX += rectWidth + 5;
		if (posX + rectWidth >= rightOffset)
		{
			posX = leftOffset;
			posY += rectHeight + 20;
		}
	}
}

int main ()
{
	const int screen_width = 800;
	const int screen_height = 600;
	player.height = 15;
	player.width = 90;
	player.x = (screen_width / 2) - (player.width / 2);
	player.y = screen_height - 50;
	player.speed = 10;
	
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.radius = 15;
	ball.speedX = 7;
	ball.speedY = 7;
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(screen_width, screen_height, "Scuffed DX BALL");
	SetTargetFPS(60);
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// game loop
	while (!WindowShouldClose())
	{
		// drawing
		BeginDrawing();

		// Update
		player.Move();
		ball.Move();
		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(DARKGRAY);
		
		// Draw
		player.Draw();
		ball.Draw();
		DrawScore();
		DrawBoxes();
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}