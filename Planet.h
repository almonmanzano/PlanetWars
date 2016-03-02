#ifndef __PLANET_H__
#define __PLANET_H__

#include "cocos2d.h"

class Planet : public cocos2d::Sprite
{
public:
	static Planet *create(char *string, cocos2d::Sprite *sprite);

	virtual bool init(char *string, cocos2d::Sprite *sprite);
	char *getString();
	void setString(char *string);
	cocos2d::Sprite *getSprite();
	void setSprite(cocos2d::Sprite *sprite);
	int getShips();
	void setShips(int ships);
	cocos2d::Label *getLabel();

private:
	char *_string;
	cocos2d::Sprite *_sprite;
	int _ships;
	cocos2d::Label *_label;
	float _scale;
};

#endif // __PLANET_H__
