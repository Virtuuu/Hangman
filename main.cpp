
// ---- 

#include "stdafx.h"
#include "commonFunc.h"
#include "baseObject.h"
#include "hangmanImage.h"
#include "Text.h"
#include "playerPower.h"

baseObject g_background;
hangmanImage g_player;
PlayerPower player_power;
TTF_Font* fontText = NULL;
TTF_Font* fontFinish = NULL;
TTF_Font* fontLevel = NULL;
TTF_Font* fontHangman = NULL;
Mix_Music* music = NULL;
Mix_Chunk* win = NULL;
Mix_Chunk* lost = NULL;


// khởi tạo window, renderer, mixer, ...
bool init();
// load ảnh
bool load();
// tạo vòng lặp cho game
int moreGame();
// tạo menu ban đầu cho game. người chơi chọn level
int chooseFile();

void close();

string chooseWord(int level); 												// returns a random word
string initGuessWord(string word);

// 
bool checkGuessWord(string word, char guess);
string updateGuessWord(string word, char guess, string guessWord);

int main(int argc, char* argv[])
{
	if (!init()) return -1;
	if (!load()) return -1;

	Mix_PlayMusic(music, 0);

	g_player.initSpriteRect();

	SDL_RenderClear(g_screen);
	g_background.render(g_screen, NULL);
	SDL_RenderPresent(g_screen);
	int level = chooseFile();
	int cnt_die = 0;
	int point = 0;
	string str_points = to_string(point);

	bool is_quit = false;
	if (level == -1) is_quit = true;
	while (!is_quit)
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
		markSDL.setText(str_points);
		


		note.LoadFromRenderText(fontText, g_screen);
		noteGuessWord.LoadFromRenderText(fontText, g_screen);
		noteWrongGuess.LoadFromRenderText(fontText, g_screen);
		guessWordSDL.LoadFromRenderText(fontText, g_screen);
		noteWin.LoadFromRenderText(fontText, g_screen);
		noteLost.LoadFromRenderText(fontText, g_screen);
		strSDL.LoadFromRenderText(fontText, g_screen);
		wordSDL.LoadFromRenderText(fontText, g_screen);
		markNote.LoadFromRenderText(fontText, g_screen);
		markSDL.LoadFromRenderText(fontText, g_screen);
		

		int a = noteGuessWord.getWidth();
		int b = noteWrongGuess.getWidth();
		int c = strSDL.getWidth();
		int cnt = 0;

		do
		{
			SDL_RenderClear(g_screen);
			g_background.render(g_screen, NULL);
			player_power.show(g_screen);
			markNote.renderText(g_screen, 10, 10);
			markSDL.renderText(g_screen, 10 + markNote.getWidth(), 10);
			noteGuessWord.renderText(g_screen, screen_width - 550, 300);
			noteWrongGuess.renderText(g_screen, screen_width - 550, 350);
			note.renderText(g_screen, screen_width - 550, 400);
			guessWordSDL.renderText(g_screen, screen_width - 550 + a, 300);
			wrongWordSDL.renderText(g_screen, screen_width - 550 + b, 350);
			g_player.show(g_screen, cnt);
			SDL_RenderPresent(g_screen);

			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_QUIT)
				{
					is_quit = true;
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

					guessWordSDL.LoadFromRenderText(fontText, g_screen);
					wrongWordSDL.LoadFromRenderText(fontText, g_screen);
				}
			}

		} while (guessWord != word && cnt != BAD_GUESS_MAX);
	
		
		if (guessWord == word)
		{
			noteWin.renderText(g_screen, screen_width - 420, 480);
			strSDL.renderText(g_screen, screen_width - 420, 530);
			wordSDL.renderText(g_screen, screen_width - 420 + c, 530);
			cnt = 0;
			g_player.show(g_screen, cnt);
			Mix_PauseMusic();
			Mix_PlayChannel(-1, win, 0);
			point += 10;
			str_points = to_string(point);
			
			SDL_Delay(2000);
		}
		
		else
		{
			noteLost.renderText(g_screen, screen_width - 420, 480);
			strSDL.renderText(g_screen, screen_width - 420, 530);
			wordSDL.renderText(g_screen, screen_width - 420 + c, 530);
			cnt = BAD_GUESS_MAX;
			g_player.show(g_screen, cnt);
			Mix_PauseMusic();
			Mix_PlayChannel(-1, lost, 0);
			SDL_Delay(2000);
			cnt_die++;
			
			if (cnt_die == POWER_MAX)
			{
				textObject gameOver;
				SDL_RenderClear(g_screen);
				g_background.render(g_screen);
				gameOver.setColor(textObject::BLACK_TEXT);
				gameOver.setText("GAME OVER!!!");
				gameOver.LoadFromRenderText(fontHangman, g_screen);
				gameOver.renderText(g_screen, 140, 260);
				SDL_RenderPresent(g_screen);
				SDL_Delay(3000);
				is_quit = true;

			}
			else
			{
				SDL_RenderClear(g_screen);
				g_background.render(g_screen, NULL);
				SDL_RenderPresent(g_screen);
				int more = moreGame();
				if (more == 1) is_quit = true;
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
	g_window = SDL_CreateWindow("Hangman Game!!!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen_width,
		screen_heigh,
		SDL_WINDOW_SHOWN);
	if (g_window == NULL) success = false;
	g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (g_screen == NULL) success = false;
	SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);

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
	if (!g_background.loadIMG("image//bg.jpg", g_screen)) load = false;
	if (!g_player.loadIMG("image//spritehangman.jpeg", g_screen)) load = false;
	if (!player_power.init("image//heart.png", g_screen)) load = false;
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
	moreGameInfor[0].LoadFromRenderText(fontFinish, g_screen);
	moreGameInfor[1].LoadFromRenderText(fontFinish, g_screen);

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
		moreGameInfor[0].renderText(g_screen, pos[0].x, pos[0].y);
		moreGameInfor[1].renderText(g_screen, pos[1].x, pos[1].y);

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
					moreGameInfor[i].LoadFromRenderText(fontFinish, g_screen);
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
		SDL_RenderPresent(g_screen);
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
	chooseLevel.setText("Choose Level");
	chooseLevel.LoadFromRenderText(fontLevel, g_screen);

	hangman.setColor(textObject::BLACK_TEXT);
	hangman.setText("HANGMAN");
	hangman.LoadFromRenderText(fontHangman, g_screen);

	level[0].setColor(textObject::BLACK_TEXT);
	level[0].setText("Easy");
	level[0].LoadFromRenderText(fontLevel, g_screen);

	level[1].setColor(textObject::BLACK_TEXT);
	level[1].setText("Medium");
	level[1].LoadFromRenderText(fontLevel, g_screen);

	level[2].setColor(textObject::BLACK_TEXT);
	level[2].setText("Hard");
	level[2].LoadFromRenderText(fontLevel, g_screen);

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
		hangman.renderText(g_screen, 180, 260);
		chooseLevel.renderText(g_screen, 100, 400);
		for (int i = 0; i < 3; i++)
		{
			level[i].renderText(g_screen, pos[i].x, pos[i].y);
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
					level[i].LoadFromRenderText(fontLevel, g_screen);
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
		SDL_RenderPresent(g_screen);
	}
	return -1;
}

void close()
{
	Mix_CloseAudio();
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

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
