#include "InitialScene.h"
#include "GameScene.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* InitialScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = InitialScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool InitialScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Background
	_bgSprite = Sprite::create("images/background.png");
	_bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	_bgSprite->setScale(visibleSize.width / _bgSprite->getContentSize().width,
		visibleSize.height / _bgSprite->getContentSize().height);
	this->addChild(_bgSprite);

	// Title
	auto title = Label::createWithTTF("Planet Wars", "fonts/arial.ttf", 32);
	title->setPosition(Point(visibleSize.width / 2, visibleSize.height / 5 * 3.55));
	title->setColor(cocos2d::Color3B(128, 0, 255));
	title->setScale(3);
	this->addChild(title);

	auto title2 = Label::createWithTTF("Planet Wars", "fonts/arial.ttf", 32);
	title2->setPosition(Point(visibleSize.width / 2, visibleSize.height / 5 * 3.5));
	title2->setColor(cocos2d::Color3B(255, 0, 255));
	title2->setScale(2.9);
	this->addChild(title2);

	// Menu
	auto barPlayEasy = Sprite::create("images/bluebar.png");
	barPlayEasy->setPosition(Point(visibleSize.width / 4 * 1, visibleSize.height / 7 * 3));
	this->addChild(barPlayEasy);
	auto playEasy = MenuItemFont::create("EASY", CC_CALLBACK_1(InitialScene::PlayEasy, this));
	playEasy->setPosition(barPlayEasy->getPosition());
	playEasy->setColor(cocos2d::Color3B(0, 128, 0));

	auto barPlayMedium = Sprite::create("images/bluebar.png");
	barPlayMedium->setPosition(Point(visibleSize.width / 4 * 2, visibleSize.height / 7 * 3));
	this->addChild(barPlayMedium);
	auto playMedium = MenuItemFont::create("MEDIUM", CC_CALLBACK_1(InitialScene::PlayMedium, this));
	playMedium->setPosition(barPlayMedium->getPosition());
	playMedium->setColor(cocos2d::Color3B(255, 127, 80));

	auto barPlayHard = Sprite::create("images/bluebar.png");
	barPlayHard->setPosition(Point(visibleSize.width / 4 * 3, visibleSize.height / 7 * 3));
	this->addChild(barPlayHard);
	auto playHard = MenuItemFont::create("HARD", CC_CALLBACK_1(InitialScene::PlayHard, this));
	playHard->setPosition(barPlayHard->getPosition());
	playHard->setColor(cocos2d::Color3B(255, 0, 0));

	auto barExit = Sprite::create("images/bluebar.png");
	barExit->setPosition(Point(visibleSize.width / 2, visibleSize.height / 10 * 2));
	this->addChild(barExit);
	auto exit = MenuItemFont::create("EXIT", CC_CALLBACK_1(InitialScene::Exit, this));
	exit->setPosition(barExit->getPosition());
	exit->setColor(cocos2d::Color3B(0, 0, 255));

	auto *menu = Menu::create(playEasy, playMedium, playHard, exit, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	// Music
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(INTRO_MUSIC);
	audio->playBackgroundMusic(INTRO_MUSIC, true);

	return true;
}

// Plays the game with the easy level of difficulty
void InitialScene::PlayEasy(Ref *ref)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = GameScene::createScene(EASY_LEVEL);
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Plays the game with the medium level of difficulty
void InitialScene::PlayMedium(Ref *ref)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = GameScene::createScene(MEDIUM_LEVEL);
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Plays the game with the hard level of difficulty
void InitialScene::PlayHard(Ref *ref)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	auto scene = GameScene::createScene(HARD_LEVEL);
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Exits the game
void InitialScene::Exit(Ref *ref)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
