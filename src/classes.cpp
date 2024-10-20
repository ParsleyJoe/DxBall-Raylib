#include "raylib.h"
#include "classes.hpp"

extern int lives;
extern int score;
extern paddle player;
extern Ball ball;
extern Scenes currentScene;

void paddle::Draw()
{
	DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), RAYWHITE);
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
	if (moving && active)
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
	if (CheckCollisionCircleRec(Vector2{ static_cast<float>(x), static_cast<float>(y) }, radius, Rectangle{ player.x, player.y, player.width, player.height }))
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

	if (lives <= 0)
	{
		active = false;
	}
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

		if (CheckCollisionCircleRec(Vector2{ static_cast<float>(ball.x), static_cast<float>(ball.y) }, ball.radius, Rectangle{ posX, posY, rectWidth, rectHeight }) && boxGrid[i] == 1)
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

void GameOver()
{
	int posX = (GetScreenWidth() / 2) - 40;
	int posY = GetScreenWidth() / 2;
	DrawText("Game Over!", posX, posY, 40, RAYWHITE);
}

void MainMenu()
{
	int posX = (GetScreenWidth() / 2) - 200;
	int posY = GetScreenWidth() / 2;
	DrawText("Press Enter To Start Game", posX, posY, 40, RAYWHITE);

	if (IsKeyDown(KEY_ENTER))
	{
		currentScene = Game;
	}
}