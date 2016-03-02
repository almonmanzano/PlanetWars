#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Planet.h"
#include "Ship.h"
#include "cocos2d.h"
#include <list>

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int lod);

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	// Background
	cocos2d::Sprite *bgSprite;

	// Menu
	void GoBack(Ref* pSender);
	void Pause(Ref* pSender);
	void Music(Ref *pSender);
	void Sound(Ref *pSender);

	// Touches
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

private:
	cocos2d::Vector<Planet*> _planets;
	cocos2d::Vector<Planet*> _selected;
	cocos2d::Vector<Ship*> _ships;
	cocos2d::Vector<Planet*> _redPlanets;
	cocos2d::Vector<Planet*> _noRedPlanets;
	int _allies, _enemies, _neutrals;
	cocos2d::MenuItemImage *_pause;
	cocos2d::MenuItemImage *_sound;
	cocos2d::MenuItemImage *_music;
	bool _paused, _musicMuted, _soundMuted;
	bool _playing;

	// AI
	bool _enemyHasSelected;
	cocos2d::Vector<Planet*> _enemySelected;
	void enemyAI(float dt);

	void gameUpdate(float dt);
	void produceShips(float dt);
	void coordSounds(float dt);

	void initPlanets(int min, int max);
	void initTouch();
	void select(Planet *planet);
	void sendShips(cocos2d::Vector<Planet*> attacker, Planet *attacked);
	void attack(Ship *ship);
	void checkVictory();
};

#endif // __GAME_SCENE_H__
