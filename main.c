#include <stdio.h>
#include <raylib.h>
#include "main.h"

#define CENTER 200
#define LOWERY 300
#define WINDOW_HEIGHT 450
#define WINDOW_WIDTH 750
#define FPS 60
#define BALL_SPEED 700
#define START_STOP_KEY KEY_A
#define BALL_START_KEY MOUSE_LEFT_BUTTON

void DrawTextB(const char *text, int posX, int posY, int fontSize, Color color)
{
    DrawText(text, posX+2, posY+2, fontSize, BLACK);
    DrawText(text, posX, posY, fontSize, color);
}

int main() {
	/*
	 *
	 * Initializing Windows,  with specific windowHeight, and windowWidth and targetting fps as 60
	 * fps = frame per second, refresh rate. i.e. how many times a screen updates itself (redraws the window)
	 * Init Audio device chae sound play garna audio device lai ready rakhna parxa
	 *
	 */
	int windowHeight = WINDOW_HEIGHT;
	int windowWidth = WINDOW_WIDTH;
	InitWindow(windowWidth, windowHeight, "Table Tennis");
	InitAudioDevice();
	SetTargetFPS(FPS);

	/*
	 *
	 * yo asset vanne struct le chae vibhinna asset haru store garxa, jastai
	 * dang = ball thokkida aaune sound file lai bokxa
	 * gameState = yo vaneko game ko state ho..,,,,,,  elle chae kun screen dekhaune kura निर्धारण  garxa
	 * jastai gameState = 1 vayo vane "playing" state vanam, tetikhera game khelne screen dekhaune, 0 vako bela initial screen
	 * ani 2 vako bela chae gameOver vako screen dekhaune
	 * uiImage = elle chae background image bolkxa
	 * textSpeed = yesko kei kaam xaina, euta purpose ko laagi rakhya the, hatauna birsexu, paxi kei maa kaam laage use garda hunxa
	 * logo = elle chae "ekaaki tabul tennis" lekheko game ko logo wala image bokxa
	 * gameOver = gameover ma dekhaune "khel khattam" lekheko image bokxa
	 * gameOver_sound = gameOver huda kheri aaune sound bokxa
	 *
	 *
	 */
	GameState gameState = START;

	Assets asset =
	 {.dang = LoadSound("Assets/ping.mp3"),\
     .gameState = 0, \
     .uiImage = LoadImage("Assets/background.png"),\
     .logo = LoadImage("Assets/logo.png"),\
	 .gameOver = LoadImage("Assets/gameOver.png"),\
	 .gameOver_sound = LoadSound("Assets/gameOver.wav"),\
	 .gm = gameState,\
    };
	// Game ka asset haru

	/*
	 *
	 * esle chae vae varko data haru bokxa, yei struct (data) lai sappai tira pass garne ho taaki
	 * game ko data haru access garna ra change garna sakum.
	 * (ball ko positions haru)
	 * posx = ball ko initial posx
	 * posy = ball ko iintial posy
	 * radius = ball ko radius
	 * speedX = ball ko x direction ma speed
	 * speedY  = ball ko Y direction ma speed
	 * score = kheladi ko score
	 * ass = mathi ko asset vanne struct elle hold garxa,,,, elle garda aba data->ass.gameState esto garera asset vitra ka kura haru ni acces garna milxa
	 * background = mathi ko uiImage lai texture ko roop ma store garne (screen ma dekhauna texture ko roop ma store garnu parxa)
	 * logo_tex, gameOver_tex haru ni tei ho
	 */

	Data data = { .posx = 100, .posy = 350,\
     .radius = 10, .speedX = BALL_SPEED,\
	 .speedY = BALL_SPEED,\
	 .score = 0,\
	 .ass = asset,\
	 .background = LoadTextureFromImage(asset.uiImage),\
	 .logo_tex = LoadTextureFromImage(asset.logo),\
	 .gameOver_tex = LoadTextureFromImage(asset.gameOver)\
    };
	   // Data ko property haru, sappai tira yei pathaune


	/*
	 * Yo loop chae main function ho jasle chae screen redraw gareko ko garei garxa,,,, ek second ma mathi fps jati rakhya xa teti choti
	 */
	Loop(&data);

	// Audio device close garna paro mathi ko loop na sakkun jel close hudaina. ra mathi ko loop sakine condition Esc thichne ho, tyo tala xa
	CloseAudioDevice();

	return 0 ;
}



/*
 * Yei loop ko kuro vairako thyo
 */
void Loop(Data* data)
{ while(!WindowShouldClose()) {
		Draw(data); // game vitra draw garne function
	}
}

void Draw(Data* data)
{
	/*
	 * TextFormat le chae string dinxa, yo case ma tyo score ho. Yei texformat bata aako const char * scoreFormat
	 * lai draw garne ho. Kinaki draw garne function le yo maagxa
	 *
	 *
	 *
	 *
	 */
	const char * scoreFormat = TextFormat("%d", data->score);

	/*
	 * mouseY = mouse ko Y position
	 * width = screen ko width, height = screen ko height
	 *
	 */
	int mouseY = GetMouseY();
	int width = GetScreenWidth();
	int height = GetScreenHeight();

	/*
	 * pad (bat) ko size Y ra X, ani TextColor = RED ko chae kaam xaina, tala eslai change garya xa, declare matra gare pani hunxa
	 */
	const int padSizeY = 80;
	const int padSizeX = 20;

	/*
	 *
	 * Pad lai mouse le control garna
	 * mouse ko Y position = Pad ko Y position gareko ho
	 * left pad ra right pad ko eutai Y position hunxa.
	 */

	// Dubai Pad ko Laagi
	int PadPosX = width - 10;
	int PadPosY = mouseY;

	/*
	 * height vanda partira mouse lagda ti height mai adkirakhos.,,, vitta jasto kaam garos vanera yo gareko
	 *
	 */

	if( PadPosY > height- padSizeY ) {
		PadPosY = height - padSizeY;
	}

	/*
	 *
	 * Yo chae ball ko default motion ho. x = V_x * t ani y = V_y * t gare jasto ho
	 * GetFrameTime() le chae t ko value dinxa
	 *
	 */
	 if (data->ass.gm == PLAYING) {
		 data->posx += GetFrameTime() * data->speedX;
		 data->posy += GetFrameTime() * data->speedY;
	 } else if(data->ass.gm == PAUSED) {
		 data->posx = 20 + data->radius;
		 data->posy = GetMouseY() + 40 ;
	}






	/*
	 * if(left kita right vitta ma ball thokyo) {
	 * 		speed X lai opposite bana taaki X direction ma ball ko direction change hos
	 * 		score zero banaide
	 * 		gameState 1 (playing) state xa vane game over ko sound suna
	 *    gameState 1 (playing) state xa vane gameState 2 (gameOver) state paarde
	 *    ball thokkine aawaz pani suna
	 * } else if ( ball le right pad lai hanyo ) {
	 *    X speed opposite paar
	 *    ball thokiyeko sound suna
	 *    score ma 15 thapde
	 * } else if( ball le left pad lai hanyo ) {
	 * 		right pad lai handa j gareko thyo tei gar
	 * } else if (ball tala thokkiyo) {
	 * 		Y ko velocity lai opposite paarne ani sound bajaune
	 * } else if (ball mathi thokkiyo) {
	 * 		Y ko velocity lai opposite paarne ani sound bajaune
	 * }
	 *
	 *
	 *
	 *
	 */
	int n=0;
	if (data->posx >= width - data->radius || data->posx <= data->radius / 2) {
		data->speedX = -data->speedX;
		if (data->posx <= data->radius / 2) {

			data->ass.gm == PLAYING ? PlaySound(data->ass.gameOver_sound) : printf("");
			data->ass.gm == PLAYING ? data->ass.gm = GAMEOVER : printf("") ;
			PlaySound(data->ass.dang);
		}
	}
	// else if(data->posx <= PadPosX && data->posx >= PadPosX - padSizeX  && ( data->posy <= PadPosY + padSizeY && data->posy >= PadPosY)) {
	// 	data->speedX = -data->speedX;
	// 	PlaySound(data->ass.dang);
	// 	//data->score += 15;
	// }
	else if(data->posx >= 0 && data-> posx <= padSizeX + data->radius /2 && ( data->posy <= PadPosY + padSizeY && data->posy >= PadPosY)) {
		data->speedX = -data->speedX;
		PlaySound(data->ass.dang);
		n++;
		if(n>0){
		data->score += 15;
		}
	}
	else if(data->posy >= height - data->radius) {
		data->speedY = -data->speedY;
		PlaySound(data->ass.dang);
	}
	else if(data->posy <= 0) {
		data->speedY = -data->speedY;
		PlaySound(data->ass.dang);
	}

	/*
	 *
	 *
	 * gameState = yo vaneko game ko state ho..,,,,,,  elle chae kun screen dekhaune kura निर्धारण  garxa
	 * jastai gameState = 1 vayo vane "playing" state vanam, tetikhera game khelne screen dekhaune, 0 vako bela initial screen
	 * ani 2 vako bela chae gameOver vako screen dekhaune
	 *
	 *
	 */

	if(data->ass.gm == PLAYING || data->ass.gm == PAUSED)
	 {
		if(IsMouseButtonDown(BALL_START_KEY))
		{
			data->score = 0;
			data->ass.gm = PLAYING;

		}
		// Yo chae main game khelda dekhine kuro ho
		BeginDrawing();
			ClearBackground(BLACK);
			DrawTexture(data->background, 0, 0, GRAY); //background texture
			DrawLineMiddle();
			DrawText(scoreFormat, width/2 + 51, 21, 40, BLACK);  //score draw garne
			DrawText(scoreFormat, width/2 + 50, 20, 40, GREEN); //score ko laagi shadow effect
			DrawRectangle(740, 0, 10, GetScreenHeight(), GREEN);
            DrawRectangle(0, 0, 5, GetScreenHeight(), RED);
			DrawRectangle(0, 0, GetScreenWidth(), 4, GREEN);
			DrawRectangle(0,446, GetScreenWidth(), 4, GREEN);
			//DrawRectangle(0,10, 10, GetScreenHeight(), GREEN);
            //DrawRectangle(0, height-10, 10, GetScreenHeight(), GREEN);

			DrawRectangle(0, PadPosY, padSizeX, padSizeY, BLUE); //agadiko bat
			DrawRectangle(-2, PadPosY+2, padSizeX, padSizeY, BLACK); //agadiko bat ko lagi shadow
			DrawCircle(data->posx+1, data->posy+1, data->radius, BLACK); //ball
			DrawCircle(data->posx, data->posy, data->radius, WHITE); //ball ko lagi shadow
		EndDrawing();
	} else if(data->ass.gm == START) {
		DrawUI(data);
	} else if(data->ass.gm == GAMEOVER)	{
		DrawGameOverUI(data);
	}
}


/*
 * Game khelda ko bich ko line draw garne function
 *
 */

void DrawLineMiddle()
{
	Vector2 startPos = { .x = GetScreenWidth()/ (float) 2, .y = GetScreenHeight()};
	Vector2 endPos = { .x = GetScreenWidth()/ (float) 2, .y = 0 };
	DrawLineEx(startPos, endPos, 5, BLUE);
}

/*
 * Game suru huda dekhaune kuro
 *
 */

void DrawUI(Data *data) {
	if(IsKeyDown(START_STOP_KEY)) {
		data->ass.gm = PAUSED;

	}
	// left mouse button thichema ma gameState change garne

	BeginDrawing();
	ClearBackground(BLACK);
	Loading(data);
	DrawTexture(data->background, 0, 0, GRAY);
	DrawTexture(data->logo_tex, 150, -100, WHITE);
	Loading(data);
	// DrawText("Project Group: Darshan, Anmol, Aashish, Aavahan", 202, 50, 20, WHITE);
	// DrawText("Click Anywhere To Start", 202, 402, 40, BLACK);

	EndDrawing();
}

/*
 * Game over huda kheri dekhaune function
 *
 */

void DrawGameOverUI(Data *data) {
	if(IsKeyDown(START_STOP_KEY)) {
		data->ass.gm = PAUSED;
		data->posx = 30;
		data->posy = GetMouseY();
	}
	BeginDrawing();
	ClearBackground(BLACK);
	DrawTexture(data->background, 0, 0, GRAY);
	DrawTexture(data->gameOver_tex, CENTER, -100, WHITE);
	DrawTextB("PRESS SPACEBAR TO RESTART", CENTER, LOWERY + 50, 25, WHITE);
	DrawTextB(TextFormat("TOTAL SCORE: %d",data->score), CENTER + 20, LOWERY, 40, PURPLE);
	DrawTextB(TextFormat("TOTAL SCORE: %d",data->score), CENTER + 20, LOWERY, 40, MAGENTA);
    DrawTextB("Press Exc to Quit", 10, 20, 20, WHITE);
	EndDrawing();
}


void Loading(Data *data)
{
    DrawTextB(GetTime() <= 1 ? "Loading..." : "Welcome", 100 * GetTime(), 270, 40, WHITE);

    if(GetTime() >= 1)
    {
            if(GetTime() <= 2){
//            DrawTextB("Project Group: Darshan, Aashish, Aavahan, Anmol", 202, 50, 19, WHITE);
            }
            DrawTextB("LEFT CLICK TO START BALL", 150, LOWERY + 80, 20, WHITE);
            DrawTextB("PRESS SPACEBAR TO START THE GAME", 150, LOWERY + 50, 25, WHITE);
            DrawTextB("Press Esc to Exit", 10, 20, 30, WHITE);
    }
}

