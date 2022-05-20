
#ifndef PLAYER_POWER_H
#define PLAYER_POWER_H

#include "commonFunc.h"
#include "baseObject.h"

class PlayerPower : public baseObject
{
public: 
	PlayerPower();
	~PlayerPower();

	void setNum(const int& num) { number_ = num; }
	void addPos(const int& xpos);
	void show(SDL_Renderer* screen);
	bool init(string path, SDL_Renderer* screen);
	void decrease();

private:
	int number_;
	vector <int> pos_list;
};



#endif // !PLAYER_POWER_H

