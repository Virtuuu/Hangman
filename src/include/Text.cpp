
#include "stdafx.h"
#include "text.h"

textObject::textObject()
{
	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	texture = NULL;
	width_ = 0;
	height_ = 0;
}

textObject:: ~textObject()
{
	free();
}

bool textObject::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, string_val_.c_str(), text_color);
	if (text_surface)
	{
		texture = SDL_CreateTextureFromSurface(screen, text_surface);
		width_ = text_surface->w;
		height_ = text_surface->h;
		SDL_FreeSurface(text_surface);
	}
	return texture != NULL;
}

void textObject::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

void textObject::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	text_color.r = red;
	text_color.g = green;
	text_color.b = blue;
}

void textObject::setColor(int type)
{
	if (type == RED_TEXT)
	{
		SDL_Color color = { 255, 0, 0 };
		text_color = color;
	}
	else if (type == YELLOW_TEXT)
	{
		SDL_Color color = { 255, 255, 72 };
		text_color = color;
	}
	else if (type == BLACK_TEXT)
	{
		SDL_Color color = { 0, 79, 79 };
		text_color = color;
	}	
}

void textObject::renderText(SDL_Renderer* screen, int xp, int yp)
{
	SDL_Rect renderQuad = { xp, yp, width_, height_ };
	SDL_RenderCopy(screen, texture, NULL, &renderQuad);
}