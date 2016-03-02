#ifndef __SHIP_H__
#define __SHIP_H__

#include "Planet.h"
#include "cocos2d.h"

class Ship : public cocos2d::Sprite
{
public:
	static Ship *create(char *string, cocos2d::Sprite *sprite, Planet *planet);

	virtual bool init(char *string, cocos2d::Sprite *sprite, Planet *planet);
	char *getString();
	cocos2d::Sprite *getSprite();
	Planet *getPlanet();

private:
	char *_string;
	cocos2d::Sprite *_sprite;
	Planet *_planet;
	float _scale;
};

#endif // __SHIP_H__
