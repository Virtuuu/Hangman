
#ifndef HANGMAN_IMAGE_H_
#define HANGMAN_IMAGE_H_

#include "commonFunc.h"
#include "baseObject.h"

class hangmanImage : public baseObject
{
public:
	hangmanImage();
	~hangmanImage();

	bool loadIMG(std::string path, SDL_Renderer* screen);
	void initSpriteRect();
	void show(SDL_Renderer* des, int y);
	SDL_Rect getSpriteRect(int y);

protected:
	int x_pos;
	int y_pos;

	int width_sprite;
	int height_sprite;

	SDL_Rect rect[10];
	
	int sprite;
};

#endif
