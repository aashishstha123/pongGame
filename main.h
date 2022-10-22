#ifndef MAIN_H

#define MAIN_H
#include <raylib.h>

typedef enum {
	INTRO, START, PLAYING, PAUSED, GAMEOVER
} GameState;


typedef struct{
	int gameState;
	Sound dang;
	Sound gameOver_sound;
	Image uiImage;
	Image logo;
	Image gameOver;
	GameState gm;
} Assets;


typedef struct {
  int speedX;
  int speedY;
  float radius;
  int posx;
  int posy;
  int score;
  Assets ass;
  Texture2D background;
  Texture2D logo_tex;
  Texture2D gameOver_tex;
} Data;


void Loop(Data* data); 
void Draw(Data* data);
void DrawUI(Data* data);
void DrawLineMiddle();
void UpdateBall(Data* data);
void DrawGameOverUI(Data* data);
void Loading(Data *data);

#endif
