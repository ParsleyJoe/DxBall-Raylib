#include <iostream>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "classes.hpp"

enum Scenes {
	Main, Game, gameOver
};
int lives = 5; // Lives
int score = 0; // Score
paddle player;
Ball ball;

int main ()
{
	const int screen_width = 800;
	const int screen_height = 600;
	InitWindow(screen_width, screen_height, "Scuffed DX BALL");
	SetTargetFPS(60);

	std::vector<int> boxGrid = { // Grid to indicate which boxes are destroyed
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1
	};
	player.height = 15;
	player.width = 90;
	player.x = (GetScreenWidth() / 2) - (player.width / 2);
	player.y = GetScreenHeight() - 50;
	player.speed = 10;
	ball.x = GetScreenWidth() / 2;
	ball.y = GetScreenHeight() / 2;
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
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
		DrawScore(score);
		DrawBoxes(boxGrid);
		DrawLives(lives);
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}