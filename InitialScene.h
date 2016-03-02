#ifndef __INITIAL_SCENE_H__
#define __INITIAL_SCENE_H__

#include "cocos2d.h"

class InitialScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(InitialScene);

	// Background
	cocos2d::Sprite *_bgSprite;
	cocos2d::Sprite *_logoSprite;

	// Menu
	void PlayEasy(cocos2d::Ref *ref);
	void PlayMedium(cocos2d::Ref *ref);
	void PlayHard(cocos2d::Ref *ref);
	void Exit(cocos2d::Ref *ref);
};

#endif // __INITIAL_SCENE_H__
