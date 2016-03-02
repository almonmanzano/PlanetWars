#include "Ship.h"
#include "Utils.h"
#include "cocos2d.h"

USING_NS_CC;

// Creates a ship with a string (file name), a sprite and a planet to attack
Ship *Ship::create(char *string, cocos2d::Sprite *sprite, Planet *planet)
{
	Ship *ship = new Ship();
	if (ship && ship->init(string, sprite, planet))
	{
		ship->autorelease();
		return ship;
	}
	CC_SAFE_DELETE(ship);
	return nullptr;
}

// Initializes the ship
bool Ship::init(char *string, cocos2d::Sprite *sprite, Planet *planet)
{
	_string = string;
	_sprite = sprite;
	_planet = planet;
	_scale = RNDBTW(0.8, 1.2);
	_sprite->setScale(_scale);

	if (!Sprite::initWithFile(_string))
	{
		return false;
	}
	return true;
}

// Gets the string
char *Ship::getString()
{
	return _string;
}

// Gets the sprite
cocos2d::Sprite *Ship::getSprite()
{
	return _sprite;
}

// Gets the planet to attack
Planet *Ship::getPlanet()
{
	return _planet;
}
