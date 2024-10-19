#pragma once
#include <vector>

class paddle
{
public:
	float x, y;
	float width, height;
	float speed;

	void Draw();
	void Move();
	void LimitMovement();
};


class Ball
{
public:
	float x, y;
	int radius;
	float speedX;
	float speedY;
	float timeStopped;
	bool moving = true;
	
	Ball::Ball();
	void Draw();
	void Move();
	void LimitMovement();
	void ResetBall();
};


void DrawScore(int score);
void DrawBoxes(std::vector<int> &boxGrid);
void DrawLives(int lives);