

#ifndef TEXT_H
#define TEXT_H

#include "commonFunc.h"

class textObject
{
public:
	textObject();
	~textObject();
	enum TEXT_COLOR
	{
		RED_TEXT = 0,
		YELLOW_TEXT = 1,
		BLACK_TEXT = 2
	};

	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setColor(int type);
	void renderText(SDL_Renderer* screen, int xp, int yp);
	void setText(const string& text) { string_val_ = text; }
	int getWidth() const { return width_; }
	int getHeight() const { return height_; };
	string GetText() const { return string_val_; }

protected:
	string string_val_;
	SDL_Color text_color;
	SDL_Texture* texture;
	int width_;
	int height_;
};

#endif