

#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "commonFunc.h"

class baseObject
{
public:
	baseObject();
	~baseObject();
	void setRect(int& x, int& y) { p_rect.x = x; p_rect.y = y; }
	SDL_Rect getRect() { return p_rect; }
	SDL_Texture* getTexture() { return p_object; }
	virtual bool loadIMG(std::string path, SDL_Renderer* screen);
	virtual void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
	void free();

protected:

	SDL_Texture* p_object;
	SDL_Rect p_rect;

};



#endif