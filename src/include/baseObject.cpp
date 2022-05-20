
#include "stdafx.h"
#include "baseObject.h"

baseObject::baseObject()
{
	p_object = NULL;
	p_rect.x = 0;
	p_rect.y = 0;
	p_rect.w = 0;
	p_rect.h = 0;
}
baseObject:: ~baseObject()
{
	free();
}

bool baseObject::loadIMG(std::string path, SDL_Renderer* screen)
{
	SDL_Texture* new_texture = NULL;
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL)
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (new_texture != NULL)
		{
			p_rect.w = load_surface->w;
			p_rect.h = load_surface->h;
		}
		SDL_FreeSurface(load_surface);
	}
	p_object = new_texture;
	return p_object != NULL;
}

void baseObject::render(SDL_Renderer* des, const SDL_Rect* clip /*= NULL*/)
{
	SDL_Rect renderqua = { p_rect.x, p_rect.y, p_rect.w, p_rect.h };
	SDL_RenderCopy(des, p_object, clip, &renderqua);

}

void baseObject::free()
{
	if (p_object != NULL)
	{
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		p_rect.w = 0;
		p_rect.h = 0;
	}
}