#include "GameOverScene.h"
#include "InitialScene.h"
#include "GameScene.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

int golevel;

// Creates the scene with a level of difficulty "lod"
Scene* GameOverScene::createScene(int lod)
{
	// Level of difficulty
	golevel = lod;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Game Over label
	auto gameOver = Label::createWithTTF("Game Over!", "fonts/arial.ttf", 32);
	gameOver->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 3));
	gameOver->setColor(cocos2d::Color3B(255, 0, 0));
	gameOver->setScale(2);
	this->addChild(gameOver);

	// Menu
	auto retry = MenuItemFont::create("Retry", CC_CALLBACK_1(GameOverScene::Retry, this));
	retry->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 2));

	auto goBack = MenuItemFont::create("Go Back", CC_CALLBACK_1(GameOverScene::GoBack, this));
	goBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 1));

	auto *menu = Menu::create(retry, goBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	// Music
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(GAME_OVER_MUSIC);
	audio->playBackgroundMusic(GAME_OVER_MUSIC, true);

	return true;
}

// Plays the game again
void GameOverScene::Retry(cocos2d::Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = GameScene::createScene(golevel);
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Goes back
void GameOverScene::GoBack(cocos2d::Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = InitialScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}
