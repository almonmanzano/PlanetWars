#include "Planet.h"
#include "Utils.h"
#include "cocos2d.h"

USING_NS_CC;

// Creates a planet with a string (file name) and a sprite
Planet *Planet::create(char *string, cocos2d::Sprite *sprite)
{
	Planet *planet = new Planet();
	if (planet && planet->init(string, sprite))
	{
		planet->autorelease();
		return planet;
	}
	CC_SAFE_DELETE(planet);
	return nullptr;
}

// Initializes the planet
bool Planet::init(char *string, cocos2d::Sprite *sprite)
{
	_string = string;
	_sprite = sprite;
	_ships = 0;
	_label = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 32);
	_label->setPosition(_sprite->getPosition());
	_scale = RNDBTW(0.8, 1.2);
	_sprite->setScale(_scale);
	_label->setScale(_scale);

	if (!Sprite::initWithFile(_string))
	{
		return false;
	}
	return true;
}

// Gets the string
char *Planet::getString()
{
	return _string;
}

// Sets the string
void Planet::setString(char *string)
{
	_string = string;
	_sprite->setTexture(_string);
}

// Gets the sprite
cocos2d::Sprite *Planet::getSprite()
{
	return _sprite;
}

// Sets the sprite
void Planet::setSprite(cocos2d::Sprite *sprite)
{
	_sprite = sprite;
}

// Gets the number of ships
int Planet::getShips()
{
	return _ships;
}

// Sets the number of ships
void Planet::setShips(int ships)
{
	_ships = ships;
	cocos2d::String *string = String::createWithFormat("%i", _ships);
	_label->setString(string->getCString());
}

// Gets the label with the number of ships
cocos2d::Label *Planet::getLabel()
{
	return _label;
}
