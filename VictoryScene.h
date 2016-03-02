#ifndef __VICTORY_SCENE_H__
#define __VICTORY_SCENE_H__

#include "cocos2d.h"

class VictoryScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int lod);

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(VictoryScene);

	// Sprite
	cocos2d::Sprite *mySprite;

	// Menu
	void Retry(Ref* pSender);
	void GoBack(Ref* pSender);
};

#endif // __VICTORY_SCENE_H__
