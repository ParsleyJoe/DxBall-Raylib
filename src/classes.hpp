#pragma once
#include <vector>

class paddle
{
public:
	float x, y;
	float width, height;
	int speed;

	void Draw();
	void Move();
	void LimitMovement();
};


class Ball
{
public:
	int x, y;
	int radius;
	float speedX;
	float speedY;
	float timeStopped;
	bool moving = true;
	bool active = true;
	
	Ball::Ball();
	void Draw();
	void Move();
	void LimitMovement();
	void ResetBall();
};
enum Scenes {
	Main, Game
};


void DrawScore(int score);
void DrawBoxes(std::vector<int> &boxGrid);
void DrawLives(int lives);
void GameOver();
void MainMenu();