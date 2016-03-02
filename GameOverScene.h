#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int lod);

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

	// Sprite
	cocos2d::Sprite *mySprite;

	// Menu
	void Retry(Ref* pSender);
	void GoBack(Ref* pSender);
};

#endif // __GAME_OVER_SCENE_H__
