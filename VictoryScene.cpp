#include "VictoryScene.h"
#include "InitialScene.h"
#include "GameScene.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

int vlevel;

// Creates the scene with a level of difficulty "lod"
Scene* VictoryScene::createScene(int lod)
{
	// Level of difficulty
	vlevel = lod;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = VictoryScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool VictoryScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Victory label
	auto victory = Label::createWithTTF("Victory!", "fonts/arial.ttf", 32);
	victory->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 3));
	victory->setColor(cocos2d::Color3B(0, 255, 0));
	victory->setScale(2);
	this->addChild(victory);

	// Menu
	auto retry = MenuItemFont::create("Retry", CC_CALLBACK_1(VictoryScene::Retry, this));
	retry->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 2));

	auto goBack = MenuItemFont::create("Go Back", CC_CALLBACK_1(VictoryScene::GoBack, this));
	goBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4 * 1));

	auto *menu = Menu::create(retry, goBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	// Music
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(WIN_MUSIC);
	audio->playBackgroundMusic(WIN_MUSIC, true);
	return true;
}

// Plays the game again
void VictoryScene::Retry(cocos2d::Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = GameScene::createScene(vlevel);
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Goes back
void VictoryScene::GoBack(cocos2d::Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = InitialScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}
