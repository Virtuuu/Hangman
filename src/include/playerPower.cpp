#include "stdafx.h"
#include "playerPower.h"

PlayerPower::PlayerPower()
{
	number_ = 0;
}

PlayerPower::~PlayerPower()
{

}
void PlayerPower::addPos(const int& xpos)
{
	pos_list.push_back(xpos);
}
bool PlayerPower::init(string path, SDL_Renderer* screen)
{
	bool tmp = loadIMG(path, screen);
	if (tmp)
	{
		number_ = 3;
		if (pos_list.size() > 0)
		{
			pos_list.clear();
		}
		addPos(480);
		addPos(520);
		addPos(560);
	}
	return tmp;
}
void PlayerPower::show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list.size(); i++)
	{
		p_rect.x = pos_list.at(i);
		p_rect.y = 10;
		render(screen);
	}
}
void PlayerPower::decrease()
{
	number_--;
	pos_list.pop_back();
}