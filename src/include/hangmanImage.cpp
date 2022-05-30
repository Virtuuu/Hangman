
#include "stdafx.h"
#include "hangmanImage.h"

hangmanImage::hangmanImage()
{
	x_pos = 500;
	y_pos = 255;

	width_sprite = 0;
	height_sprite = 0;

	rect->x = 0;
	rect->y = 0;
	rect->w = 0;
	rect->h = 0;
}

hangmanImage:: ~hangmanImage()
{

}

bool hangmanImage::loadIMG(std::string path, SDL_Renderer* screen)
{
	bool tmp = baseObject::loadIMG(path, screen);
	if (tmp == true)
	{
		width_sprite = p_rect.w / 5;
		height_sprite = p_rect.h / 2;
	}
	return tmp;
}

void hangmanImage::initSpriteRect()
{
	if (width_sprite > 0 && height_sprite > 0)
	{
		rect[1].x = 0;
		rect[1].y = 0;
		rect[1].w = width_sprite;
		rect[1].h = height_sprite;

		rect[2].x = width_sprite;
		rect[2].y = 0;
		rect[2].w = width_sprite;
		rect[2].h = height_sprite;

		rect[3].x = 2 * width_sprite;
		rect[3].y = 0;
		rect[3].w = width_sprite;
		rect[3].h = height_sprite;

		rect[4].x = 3 * width_sprite;
		rect[4].y = 0;
		rect[4].w = width_sprite;
		rect[4].h = height_sprite;

		rect[5].x = 4 * width_sprite;
		rect[5].y = 0;
		rect[5].w = width_sprite;
		rect[5].h = height_sprite;

		rect[6].x = 0;
		rect[6].y = height_sprite;
		rect[6].w = width_sprite;
		rect[6].h = height_sprite;

		rect[7].x = width_sprite;
		rect[7].y = height_sprite;
		rect[7].w = width_sprite;
		rect[7].h = height_sprite;

		rect[8].x = 2 * width_sprite;
		rect[8].y = height_sprite;
		rect[8].w = width_sprite;
		rect[8].h = height_sprite;

		rect[9].x = 3 * width_sprite;
		rect[9].y = height_sprite;
		rect[9].w = width_sprite;
		rect[9].h = height_sprite;

		rect[0].x = 4 * width_sprite;
		rect[0].y = height_sprite;
		rect[0].w = width_sprite;
		rect[0].h = height_sprite;
	}
}

SDL_Rect hangmanImage::getSpriteRect(int y)
{
	switch (y)
	{
		case 0:
		{
			return rect[0];
			break;
		}
		case 1:
		{
			return rect[1];
			break;
		}
		case 2:
		{
			return rect[2];
			break;
		}
		case 3:
		{
			return rect[3];
			break;
		}
		case 4:
		{
			return rect[4];
			break;
		}
		case 5:
		{
			return rect[5];
			break;
		}
		case 6:
		{
			return rect[6];
			break;
		}
		case 7:
		{
			return rect[7];
			break;
		}
		case 8:
		{
			return rect[8];
			break;
		}
		default:
			return rect[9];
			break;
	}
}

void hangmanImage::show(SDL_Renderer* des, int y)
{
	SDL_Rect destRect = { x_pos, y_pos, width_sprite, height_sprite };
	SDL_Rect scrRect = getSpriteRect(y);
	SDL_RenderCopy(des, p_object, &scrRect, &destRect);
	SDL_RenderPresent(des);
}







