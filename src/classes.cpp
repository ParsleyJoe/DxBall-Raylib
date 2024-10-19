#include "raylib.h"
#include "classes.hpp"

extern int lives;
extern int score;
extern paddle player;
extern Ball ball;

void paddle::Draw()
{
	DrawRectangle(x, y, width, height, RAYWHITE);
}
void paddle::Move()
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
void paddle::LimitMovement()
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


Ball::Ball()
{
	Ball::x = GetScreenWidth() / 2;
	Ball::y = GetScreenHeight() / 2;
	timeStopped = 0;
	radius = 15;
	speedX = 5;
	speedY = 5;
}
void Ball::Draw()
{
	DrawCircle(x, y, radius, RAYWHITE);
}
void Ball::Move()
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

	Ball::LimitMovement();
}
void Ball::LimitMovement()
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
	if (CheckCollisionCircleRec(Vector2{ x, y }, radius, Rectangle{ player.x, player.y, player.width, player.height }))
	{
		speedY *= -1;
	}
}
// Reset Ball and Deduct Lives
void Ball::ResetBall()
{
	int xDir[2] = { 1, -1 };
	int yDir[2] = { 1, -1 };
	x = GetScreenWidth() / 2;
	y = GetScreenHeight() / 2;

	speedX += GetRandomValue(xDir[0], xDir[1]);
	speedY += GetRandomValue(yDir[0], xDir[1]);
	lives--;
}

void DrawScore(int score)
{
	float posX = 50;
	float posY = 20;
	DrawText(TextFormat("Score: %i", score), posX, posY, 40, RAYWHITE);
}

void DrawLives(int lives)
{
	float posX = GetScreenWidth() - 180;
	float posY = 20;
	DrawText(TextFormat("Lives: %i", lives), posX, posY, 40,RAYWHITE);
}

// Checks Collisions too
void DrawBoxes(std::vector<int> &boxGrid)
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