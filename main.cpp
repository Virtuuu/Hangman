
// ---- 

#include "stdafx.h"
#include "commonFunc.h"
#include "baseObject.h"
#include "hangmanImage.h"
#include "Text.h"
#include "playerPower.h"

baseObject gBackground;
hangmanImage gPlayer;
PlayerPower player_power;
TTF_Font* fontText = NULL;
TTF_Font* fontFinish = NULL;
TTF_Font* fontLevel = NULL;
TTF_Font* fontHangman = NULL;
Mix_Music* music = NULL;
Mix_Chunk* win = NULL;
Mix_Chunk* lost = NULL;


// initialize all game assets
bool init();
bool load();

// replay
int moreGame();

// choose difficulty
int chooseFile();

void close();

string chooseWord(int level); 												// returns a random word
string initGuessWord(string word);

// check for results & update game state
bool checkGuessWord(string word, char guess);
string updateGuessWord(string word, char guess, string guessWord);

int main(int argc, char* argv[])
{
	if (!init()) return -1;
	if (!load()) return -1;

	Mix_PlayMusic(music, 0); // BGM

	gPlayer.initSpriteRect(); 

	SDL_RenderClear(gScreen);
	gBackground.render(gScreen, NULL);
	SDL_RenderPresent(gScreen);
	int level = chooseFile();
	int cnt_die = 0;
	int point = 0;
	string strPoints = to_string(point);

	bool isQuit = false;
	if (level == -1) isQuit = true;
	while (!isQuit)
	{
		srand(time(0));

		string word = chooseWord(level);
		string guessWord = initGuessWord(word);
		char guess;
		string wrongWord;

		textObject wordSDL;
		textObject noteWin;
		textObject noteLost;
		textObject strSDL;
		textObject note;
		textObject noteGuessWord;
		textObject noteWrongGuess;
		textObject guessWordSDL;
		textObject wrongWordSDL;
		textObject markNote;
		textObject markSDL;

		note.setColor(textObject::BLACK_TEXT);
		noteGuessWord.setColor(textObject::BLACK_TEXT);
		noteWrongGuess.setColor(textObject::BLACK_TEXT);
		guessWordSDL.setColor(textObject::BLACK_TEXT);
		wrongWordSDL.setColor(textObject::BLACK_TEXT);
		noteWin.setColor(textObject::BLACK_TEXT);
		noteLost.setColor(textObject::BLACK_TEXT);
		strSDL.setColor(textObject::BLACK_TEXT);
		wordSDL.setColor(textObject::BLACK_TEXT);
		markNote.setColor(textObject::BLACK_TEXT);
		markSDL.setColor(textObject::BLACK_TEXT);



		note.setText("Enter your guess:  ");
		noteGuessWord.setText("WORD: ");
		noteWrongGuess.setText("Mistakes: ");
		guessWordSDL.setText(guessWord);
		noteWin.setText("CONGRATULATIONS!!!");
		noteLost.setText("YOU ARE HANGED! GAME OVER!!");
		strSDL.setText("The Word is: ");
		wordSDL.setText(word);
		markNote.setText("POINTS: ");
		markSDL.setText(strPoints);
		


		note.LoadFromRenderText(fontText, gScreen);
		noteGuessWord.LoadFromRenderText(fontText, gScreen);
		noteWrongGuess.LoadFromRenderText(fontText, gScreen);
		guessWordSDL.LoadFromRenderText(fontText, gScreen);
		noteWin.LoadFromRenderText(fontText, gScreen);
		noteLost.LoadFromRenderText(fontText, gScreen);
		strSDL.LoadFromRenderText(fontText, gScreen);
		wordSDL.LoadFromRenderText(fontText, gScreen);
		markNote.LoadFromRenderText(fontText, gScreen);
		markSDL.LoadFromRenderText(fontText, gScreen);
		

		int a = noteGuessWord.getWidth();
		int b = noteWrongGuess.getWidth();
		int c = strSDL.getWidth();
		int cnt = 0;

		do
		{
			SDL_RenderClear(gScreen);
			gBackground.render(gScreen, NULL);
			player_power.show(gScreen);
			markNote.renderText(gScreen, 10, 10);
			markSDL.renderText(gScreen, 10 + markNote.getWidth(), 10);
			noteGuessWord.renderText(gScreen, screen_width - 550, 300);
			noteWrongGuess.renderText(gScreen, screen_width - 550, 350);
			note.renderText(gScreen, screen_width - 550, 400);
			guessWordSDL.renderText(gScreen, screen_width - 550 + a, 300);
			wrongWordSDL.renderText(gScreen, screen_width - 550 + b, 350);
			gPlayer.show(gScreen, cnt);
			SDL_RenderPresent(gScreen);

			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_QUIT)
				{
					isQuit = true;
					return 0;
				}
				if (g_event.type == SDL_KEYDOWN)
				{
					guess = (char)g_event.key.keysym.sym;
					if (checkGuessWord(word, guess))
					{
						guessWord = updateGuessWord(word, guess, guessWord);
					}
					else
					{
						cnt++;
						wrongWord.push_back(guess);
						wrongWord.push_back(' ');
					}
					guessWordSDL.setText(guessWord);
					wrongWordSDL.setText(wrongWord);

					guessWordSDL.LoadFromRenderText(fontText, gScreen);
					wrongWordSDL.LoadFromRenderText(fontText, gScreen);
				}
			}

		} while (guessWord != word && cnt != BAD_GUESS_MAX);
	
		
		if (guessWord == word) // Victory Screen
		{
			noteWin.renderText(gScreen, screen_width - 420, 480);
			strSDL.renderText(gScreen, screen_width - 420, 530);
			wordSDL.renderText(gScreen, screen_width - 420 + c, 530);
			cnt = 0;
			gPlayer.show(gScreen, cnt);
			Mix_PauseMusic();
			Mix_PlayChannel(-1, win, 0);
			point += 10;
			strPoints = to_string(point);
			
			SDL_Delay(2000);
		}
		
		else	// Failed Screen
		{
			noteLost.renderText(gScreen, screen_width - 420, 480);
			strSDL.renderText(gScreen, screen_width - 420, 530);
			wordSDL.renderText(gScreen, screen_width - 420 + c, 530);
			cnt = BAD_GUESS_MAX;
			gPlayer.show(gScreen, cnt);
			Mix_PauseMusic();
			Mix_PlayChannel(-1, lost, 0);
			SDL_Delay(2000);
			cnt_die++;
			
			if (cnt_die == POWER_MAX)
			{
				textObject gameOver;
				SDL_RenderClear(gScreen);
				gBackground.render(gScreen);
				gameOver.setColor(textObject::BLACK_TEXT);
				gameOver.setText("Game Over!!!");
				gameOver.LoadFromRenderText(fontHangman, gScreen);
				gameOver.renderText(gScreen, 140, 260);
				SDL_RenderPresent(gScreen);
				SDL_Delay(3000);
				isQuit = true;

			}
			else
			{
				SDL_RenderClear(gScreen);
				gBackground.render(gScreen, NULL);
				SDL_RenderPresent(gScreen);
				int more = moreGame();
				if (more == 1) isQuit = true;
				player_power.decrease();
			}
		}
		Mix_ResumeMusic();
	}
	close();
	return 0;
}

// Initialize all assets

bool init()
{
	// Render window
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) success = false;
	g_window = SDL_CreateWindow("Play Hangman",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen_width,
		screen_heigh,
		SDL_WINDOW_SHOWN);
	if (g_window == NULL) success = false;
	gScreen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (gScreen == NULL) success = false;
	SDL_SetRenderDrawColor(gScreen, 255, 255, 255, 255);

	// Fonts
	if (TTF_Init() < 0) success = false;
	fontText = TTF_OpenFont("font//comicbd.ttf", 15);
	if (fontText == NULL) success = false;
	fontFinish = TTF_OpenFont("font//Family-nR2ag.otf", 40);
	if (fontFinish == NULL) success = false;
	fontLevel = TTF_OpenFont("font//comicbd.ttf", 26);
	if (fontFinish == NULL) success = false;
	fontHangman = TTF_OpenFont("font//BuxtonSketch.ttf", 70);
	if (fontHangman == NULL) success = false;

	// SFX
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == true) success = false;
	music = Mix_LoadMUS("music//bg.wav");
	win = Mix_LoadWAV("music//win_sfx.mp3");
	lost = Mix_LoadWAV("music//gameover.mp3");
	if (music == NULL || win == NULL || lost == NULL) success = false;

	return success;
}


bool load()
{
	bool load = true;
	if (!gBackground.loadIMG("image//bg.jpg", gScreen)) load = false;
	if (!gPlayer.loadIMG("image//spritehangman.jpeg", gScreen)) load = false;
	if (!player_power.init("image//heart.png", gScreen)) load = false;
	return load;
}

int moreGame()
{
	SDL_Rect pos[2];
	pos[0].x = 150;
	pos[0].y = 350;

	pos[1].x = 400;
	pos[1].y = 350;

	textObject moreGameInfor[2];

	moreGameInfor[0].setColor(textObject::BLACK_TEXT);
	moreGameInfor[1].setColor(textObject::BLACK_TEXT);
	moreGameInfor[0].setText("RETRY");
	moreGameInfor[1].setText("EXIT");
	moreGameInfor[0].LoadFromRenderText(fontFinish, gScreen);
	moreGameInfor[1].LoadFromRenderText(fontFinish, gScreen);

	bool selected[2] = { 0, 0 };

	int width_[2], height_[2];
	for (int i = 0; i < 2; i++)
	{
		width_[i] = moreGameInfor[i].getWidth();
		height_[i] = moreGameInfor[i].getHeight();
	}

	int xm = 0;
	int ym = 0;
	SDL_Event m_event;
	while (true)
	{
		moreGameInfor[0].renderText(gScreen, pos[0].x, pos[0].y);
		moreGameInfor[1].renderText(gScreen, pos[1].x, pos[1].y);

		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return 1;
			case SDL_MOUSEMOTION:
			{
				xm = m_event.motion.x;
				ym = m_event.motion.y;
				for (int i = 0; i < 2; i++)
				{
					if (xm >= pos[i].x && xm <= pos[i].x + width_[i] &&
						ym >= pos[i].y && ym <= pos[i].y + height_[i])
					{
						if (selected[i] == false)
						{
							selected[i] = 1;
							moreGameInfor[i].setColor(textObject::BLACK_TEXT);
						}
					}

					else
					{
						if (selected[i] == true)
						{
							selected[i] = 0;
							moreGameInfor[i].setColor(textObject::BLACK_TEXT);
						}
					}
					moreGameInfor[i].LoadFromRenderText(fontFinish, gScreen);
				}
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				xm = m_event.button.x;
				ym = m_event.button.y;

				for (int i = 0; i < 2; i++)
				{
					if (xm >= pos[i].x && xm <= pos[i].x + width_[i] &&
						ym >= pos[i].y && ym <= pos[i].y + height_[i])
					{

						return i;
					}
				}
				break;
			}

			default:
				break;
			}
		}
		SDL_RenderPresent(gScreen);
	}
	return 1;
}

int chooseFile()
{
	string STRING_PATH;
	SDL_Rect pos[3];
	pos[0].x = 400;
	pos[0].y = 400;

	pos[1].x = 400;
	pos[1].y = 450;

	pos[2].x = 400;
	pos[2].y = 500;

	textObject chooseLevel;
	textObject hangman;
	textObject level[3];

	chooseLevel.setColor(textObject::BLACK_TEXT);
	chooseLevel.setText("Choose Level: ");
	chooseLevel.LoadFromRenderText(fontLevel, gScreen);

	hangman.setColor(textObject::BLACK_TEXT);
	hangman.setText("HANGMAN");
	hangman.LoadFromRenderText(fontHangman, gScreen);

	level[0].setColor(textObject::BLACK_TEXT);
	level[0].setText("Easy");
	level[0].LoadFromRenderText(fontLevel, gScreen);

	level[1].setColor(textObject::BLACK_TEXT);
	level[1].setText("Medium");
	level[1].LoadFromRenderText(fontLevel, gScreen);

	level[2].setColor(textObject::BLACK_TEXT);
	level[2].setText("Hard");
	level[2].LoadFromRenderText(fontLevel, gScreen);

	bool selected[3] = { 0, 0, 0 };
	int width_[3], height_[3];
	for (int i = 0; i < 3; i++)
	{
		width_[i] = level[i].getWidth();
		height_[i] = level[i].getHeight();
	}
	int xm = 0;
	int ym = 0;

	SDL_Event m_event;
	while (true)
	{
		hangman.renderText(gScreen, 180, 260);
		chooseLevel.renderText(gScreen, 100, 400);
		for (int i = 0; i < 3; i++)
		{
			level[i].renderText(gScreen, pos[i].x, pos[i].y);
		}
		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return -1;
			case SDL_MOUSEMOTION:
			{
				xm = m_event.motion.x;
				ym = m_event.motion.y;
				for (int i = 0; i < 3; i++)
				{
					if (xm >= pos[i].x && xm <= pos[i].x + width_[i] &&
						ym >= pos[i].y && ym <= pos[i].y + height_[i])
					{
						if (selected[i] == false)
						{
							selected[i] = 1;
							level[i].setColor(textObject::BLACK_TEXT);
						}
					}
					else
					{
						if (selected[i] == true)
						{
							selected[i] = 0;
							level[i].setColor(textObject::BLACK_TEXT);
						}
					}
					level[i].LoadFromRenderText(fontLevel, gScreen);
				}
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				xm = m_event.button.x;
				ym = m_event.button.y;
				for (int i = 0; i < 3; i++)
				{
					if (xm >= pos[i].x && xm <= pos[i].x + width_[i] &&
						ym >= pos[i].y && ym <= pos[i].y + height_[i])
					{
						return i;
					}
				}
				break;
			}

			default:
				break;
			}
		}
		SDL_RenderPresent(gScreen);
	}
	return -1;
}

void close()
{
	Mix_CloseAudio();
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	SDL_DestroyRenderer(gScreen);
	gScreen = NULL;

	IMG_Quit();
	SDL_Quit();
}

string chooseWord(int level)
{
	string STRING_PATH;
	if (level == 0) STRING_PATH = "easy.txt";
	else if (level == 1) STRING_PATH = "medium.txt";
	else STRING_PATH = "hard.txt";
	vector <string> WORD_LIST;
	ifstream file(STRING_PATH);
	if (!file)
	{
		return 0;
	}
	else
	{
		while (!file.eof())
		{
			string word;
			getline(file, word);
			WORD_LIST.push_back(word);
		}
	}
	file.close();
	int number_word = WORD_LIST.size();
	int a = rand() % number_word;
	return WORD_LIST[a];
}

string initGuessWord(string word)
{
	string guessWord = string(word.length(), '*');
	return guessWord;
}

bool checkGuessWord(string word, char guess)
{
	int w = word.find_first_of(guess);
	if (w != -1) return true;
	return false;
}

string updateGuessWord(string word, char guess, string guessWord)
{
	char guessUpper = toupper(guess);
	int len = word.length();
	for (int i = 0; i < len; i++)
	{
		if (guess == word[i] || guessUpper == word[i])
		{
			guessWord[i] = guess;
		}
	}
	return guessWord;
}
