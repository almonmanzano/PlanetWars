#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "InitialScene.h"
#include "GameOverScene.h"
#include "VictoryScene.h"
#include "Utils.h"

USING_NS_CC;

int level;

// Creates the scene with a level of difficulty "lod"
Scene* GameScene::createScene(int lod)
{
	// Level of difficulty
	level = lod;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Background
	bgSprite = Sprite::create("images/starfield.png");
	bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
		visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, Z_BG);

	// Menu
	auto bar = Sprite::create("images/bluebar.png");
	bar->setPosition(Point(visibleSize.width / 2, visibleSize.height / 15 * 14));
	this->addChild(bar, Z_GUI);
	auto goBack = MenuItemFont::create("Go Back", CC_CALLBACK_1(GameScene::GoBack, this));
	goBack->setPosition(bar->getPosition());
	goBack->setColor(cocos2d::Color3B(0, 0, 255));

	_pause = MenuItemImage::create("images/pause1.png", "images/pause2.png", CC_CALLBACK_1(GameScene::Pause, this));
	_pause->setPosition(Point(visibleSize.width / 2 + goBack->getContentSize().width / 2 + _pause->getContentSize().width, visibleSize.height / 15 * 14));
	_pause->setColor(cocos2d::Color3B(255, 255, 255));

	_music = MenuItemImage::create("images/music1.png", "images/music2.png", CC_CALLBACK_1(GameScene::Music, this));
	_music->setPosition(Point(visibleSize.width / 2 - goBack->getContentSize().width / 2 - _pause->getContentSize().width, visibleSize.height / 15 * 14));
	_music->setColor(cocos2d::Color3B(255, 255, 255));

	_sound = MenuItemImage::create("images/sound1.png", "images/sound2.png", CC_CALLBACK_1(GameScene::Sound, this));
	_sound->setPosition(Point(_music->getPosition().x - _pause->getContentSize().width, visibleSize.height / 15 * 14));
	_sound->setColor(cocos2d::Color3B(255, 255, 255));

	auto *menu = Menu::create(goBack, _pause, _music, _sound, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, Z_GUI);

	// Music
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(BG_MUSIC);
	audio->playBackgroundMusic(BG_MUSIC, true);
	audio->setBackgroundMusicVolume(0.75);

	// Sounds
	audio->preloadEffect(ALLY_ATTACK);
	audio->preloadEffect(ENEMY_ATTACK);

	// Init
	initPlanets(15, 20);
	initTouch();
	_paused = false;
	_musicMuted = false;
	_soundMuted = false;

	// Update
	schedule(schedule_selector(GameScene::gameUpdate));
	schedule(schedule_selector(GameScene::produceShips), 0.75);
	schedule(schedule_selector(GameScene::coordSounds), 0.5);

	// AI based on level of difficulty
	float dt = 1.5; // <- level = EASY_LEVEL
	if (level == MEDIUM_LEVEL)
	{
		dt = 1;
	}
	if (level == HARD_LEVEL)
	{
		dt = 0.25;
	}
	schedule(schedule_selector(GameScene::enemyAI), dt);

	return true;
}

// Update the ships
void GameScene::gameUpdate(float dt)
{
	cocos2d::Vector<Ship*> shipsToDelete;

	for (Ship *ship : _ships)
	{
		if (_paused)
		{
			ship->getSprite()->pause(); // ships do not move if the game is paused
		}
		else
		{
			ship->getSprite()->resume(); // ships move if the game is not paused
			auto pos1 = ship->getSprite()->getPosition();
			auto pos2 = ship->getPlanet()->getSprite()->getPosition();
			if (pos1 == pos2) // the ship reached the attacked planet
			{
				shipsToDelete.pushBack(ship);
				attack(ship);
			}
		}
	}

	// Deleting the ships which have finished
	for (Ship *ship : shipsToDelete)
	{
		_ships.eraseObject(ship);
		this->removeChild(ship->getSprite());
	}
}

// Make the planets produce ships
void GameScene::produceShips(float dt)
{
	if (!_paused)
	{
		for (Planet *planet : _planets)
		{
			if (planet->getString() != GREY_PLANET)
			{
				planet->setShips(planet->getShips() + 1); // produces 1 ship more
			}
		}
	}
}

// Controls the AI of the enemy
void GameScene::enemyAI(float dt)
{
	if (!_paused)
	{
		if (_redPlanets.size() > 0)
		{
			int n = RNDBTW(0, 1);

			// send ships to a grey or green planet
			if (_allies + _neutrals > 0 && _enemySelected.size() > 0 && n < 0.5)
			{
				int index = RNDBTW(0, _noRedPlanets.size() - 1);
				Planet *planet = _noRedPlanets.at(index);

				for (Planet *sel : _enemySelected)
				{
					sel->setString(RED_PLANET);
				}
				sendShips(_enemySelected, planet);
				_enemySelected.clear();
			}

			// select a red planet
			else
			{
				int index = RNDBTW(0, _redPlanets.size() - 1);
				Planet *planet = _redPlanets.at(index);
				planet->setString(SELECTED_RED_PLANET);
				if (!_enemySelected.contains(planet))
				{
					_enemySelected.pushBack(planet);
				}
			}
		}
	}
}

// Prevents several sounds at once
void GameScene::coordSounds(float dt)
{
	_playing = false;
}

// Positions all the planets
void GameScene::initPlanets(int min, int max)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Sprite *planet = nullptr;
	Planet *p = nullptr;
	cocos2d::Label *label = nullptr;

	// Green Planet
	planet = Sprite::create(GREEN_PLANET);
	planet->setAnchorPoint(Point(0.5, 0.5));
	planet->setPosition(visibleSize.width - planet->getContentSize().width / 2,
	visibleSize.height - planet->getContentSize().height / 2);
	p = Planet::create(GREEN_PLANET, planet);
	_planets.pushBack(p);
	_noRedPlanets.pushBack(p);
	this->addChild(planet, Z_PLANETS);
	this->addChild(p->getLabel(), Z_INDICATORS);
	_allies = 1;

	// Red Planet
	planet = Sprite::create(RED_PLANET);
	planet->setAnchorPoint(Point(0.5, 0.5));
	planet->setPosition(planet->getContentSize().width / 2, planet->getContentSize().height / 2);
	p = Planet::create(RED_PLANET, planet);
	_planets.pushBack(p);
	_redPlanets.pushBack(p);
	this->addChild(planet, Z_PLANETS);
	this->addChild(p->getLabel(), Z_INDICATORS);
	_enemies = 1;

	// Neutral Planets
	_neutrals = 0;
	for (int i = 2; i < max - CCRANDOM_0_1() * (max - min); i++)
	{
		planet = Sprite::create(GREY_PLANET);
		planet->setAnchorPoint(Point(0.5, 0.5));

		// prevents positioning several planets at the same point
		bool flag = true;
		while (flag)
		{
			flag = false;
			planet->setPosition(
				RNDBTW(planet->getContentSize().width / 2, visibleSize.width - planet->getContentSize().width / 2),
				RNDBTW(planet->getContentSize().height / 2, visibleSize.height - planet->getContentSize().height));
			for (Planet *other : _planets)
			{
				auto pos1 = planet->getPosition();
				auto pos2 = other->getSprite()->getPosition();
				if (pos1.distance(pos2) < planet->getContentSize().width)
				{
					flag = true;
					break;
				}
			}
		}
		p = Planet::create(GREY_PLANET, planet);
		_planets.pushBack(p);
		_noRedPlanets.pushBack(p);
		this->addChild(planet, Z_PLANETS);
		this->addChild(p->getLabel(), Z_INDICATORS);
		_neutrals++;
	}
}

// Initialize the touch events
void GameScene::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

// Controls the touches
bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	for (Planet *planet : _planets)
	{
		auto planetPos = planet->getSprite()->getPosition();
		float distance = planetPos.getDistance(touch->getLocation());

		// if touching a planet
		if (distance < planet->getSprite()->getContentSize().width / 2)
		{
			select(planet);
		}
	}

	return true;
}

// Selects a planet
void GameScene::select(Planet *planet)
{
	char *string = planet->getString();

	// selecting a planet
	if (string == GREEN_PLANET)
	{
		planet->setString(SELECTED_GREEN_PLANET);
		_selected.pushBack(planet);
	}

	// attacking a planet
	else if (string != SELECTED_GREEN_PLANET && _selected.size() > 0)
	{
		for (Planet *sel : _selected)
		{
			sel->setString(GREEN_PLANET);
		}
		sendShips(_selected, planet);
		_selected.clear();
	}
}

// Sends the ships of the attacker planets to the attacked planet
void GameScene::sendShips(cocos2d::Vector<Planet*> attacker, Planet *attacked)
{
	char *type = attacker.at(0)->getString();

	cocos2d::Sprite *ship = nullptr;
	Ship *s = nullptr;

	for (Planet *planet : attacker)
	{
		int shipsAttacking = planet->getShips();

		for (int i = 0; i < shipsAttacking; i++)
		{
			if (type == GREEN_PLANET)
			{
				ship = Sprite::create(GREEN_SHIP);
			}
			else
			{
				ship = Sprite::create(RED_SHIP);
			}
			ship->setAnchorPoint(Point(0.5, 0.5));
			ship->setPosition(
				planet->getSprite()->getPosition().x + RNDBTW(-1, 1) * ship->getContentSize().width,
				planet->getSprite()->getPosition().y + RNDBTW(-1, 1) * ship->getContentSize().height);
			if (type == GREEN_PLANET)
			{
				s = Ship::create(GREEN_SHIP, ship, attacked);
			}
			else
			{
				s = Ship::create(RED_SHIP, ship, attacked);
			}
			_ships.pushBack(s);
			this->addChild(ship, Z_SHIPS);
			auto d = attacked->getSprite()->getPosition().distance(ship->getPosition());
			auto action = MoveTo::create(d / SHIP_SPEED * RNDBTW(0.8, 1.2), attacked->getSprite()->getPosition());
			ship->runAction(action);
		}
		planet->setShips(planet->getShips() - shipsAttacking);
	}
}

// Atacks a planet
void GameScene::attack(Ship *ship)
{
	Planet *planet = ship->getPlanet();

	// if the ship is ally
	if (ship->getString() == GREEN_SHIP)
	{
		if (!_soundMuted && !_playing)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(ALLY_ATTACK);
			_playing = true;
		}

		// if the planet is enemy or neutral
		if (planet->getString() == RED_PLANET || planet->getString() == SELECTED_RED_PLANET || planet->getString() == GREY_PLANET)
		{
			// conquers the planet
			if (planet->getShips() == 0)
			{
				if (planet->getString() == GREY_PLANET)
				{
					_neutrals--;
				}
				else
				{
					_enemies--;
					_redPlanets.eraseObject(planet);
					_noRedPlanets.pushBack(planet);
					if (planet->getString() == SELECTED_RED_PLANET)
					{
						_enemySelected.eraseObject(planet);
					}
				}
				_allies++;
				planet->setString(GREEN_PLANET);
				planet->setShips(1);
				checkVictory();
			}

			// reduces the number of ships of the planet
			else
			{
				planet->setShips(planet->getShips() - 1);
			}
		}

		// if the planet is ally
		else
		{
			// if the planet is ally, defends the planet
			planet->setShips(planet->getShips() + 1);
		}
	}

	// if the ship is enemy
	else
	{
		if (!_soundMuted && !_playing)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(ENEMY_ATTACK);
			_playing = true;
		}

		// if the planet is ally or neutral
		if (planet->getString() == GREEN_PLANET || planet->getString() == SELECTED_GREEN_PLANET || planet->getString() == GREY_PLANET)
		{
			// conquers the planet
			if (planet->getShips() == 0)
			{
				if (planet->getString() == GREY_PLANET)
				{
					_neutrals--;
				}
				else
				{
					_allies--;
				}
				_enemies++;
				_noRedPlanets.eraseObject(planet);
				_redPlanets.pushBack(planet);
				planet->setString(RED_PLANET);
				planet->setShips(1);
				checkVictory();
			}

			// reduces the number of ships of the planet
			else
			{
				planet->setShips(planet->getShips() - 1);
			}
		}

		// if the planet is enemy, defends the planet
		else
		{
			planet->setShips(planet->getShips() + 1);
		}
	}
}

// Checks if there is a winner
void GameScene::checkVictory()
{
	bool greenWin = true;
	bool redWin = true;

	for (Planet *planet : _planets)
	{
		if (planet->getString() != GREEN_PLANET && planet->getString() != SELECTED_GREEN_PLANET)
		{
			greenWin = false;
		}
		if (planet->getString() != RED_PLANET && planet->getString() != SELECTED_RED_PLANET)
		{
			redWin = false;
		}
	}

	// if the player wins
	if (greenWin)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		auto scene = VictoryScene::createScene(level);
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}

	// if the player loses
	if (redWin)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		auto scene = GameOverScene::createScene(level);
		Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
	}
}

// Goes back
void GameScene::GoBack(cocos2d::Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	auto scene = InitialScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

// Pauses/resumes the game
void GameScene::Pause(cocos2d::Ref *pSender)
{
	_pause->setColor(cocos2d::Color3B(255, 0, 0));
	if (_paused)
	{
		_pause->setColor(cocos2d::Color3B(255, 255, 255));
	}
	_paused = !_paused;
}

// Mutes/adds sound
void GameScene::Sound(cocos2d::Ref *pSender)
{
	if (_soundMuted)
	{
		_sound->setColor(cocos2d::Color3B(255, 255, 255));
	}
	else
	{
		_sound->setColor(cocos2d::Color3B(255, 0, 0));
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	}
	_soundMuted = !_soundMuted;
}

// Pauses/resumes music
void GameScene::Music(cocos2d::Ref *pSender)
{
	if (_musicMuted)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		_music->setColor(cocos2d::Color3B(255, 255, 255));
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		_music->setColor(cocos2d::Color3B(255, 0, 0));
	}
	_musicMuted = !_musicMuted;
}
