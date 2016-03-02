#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define RNDBTW(min, max) (rand() % (int)(max - min + 1) + min)

#define GREY_PLANET "images/planet_grey.png"
#define GREEN_PLANET "images/planet_green.png"
#define SELECTED_GREEN_PLANET "images/planet_green_selected.png"
#define RED_PLANET "images/planet_red.png"
#define SELECTED_RED_PLANET "images/planet_red_selected.png"
#define GREEN_SHIP "images/ufo_green.png"
#define RED_SHIP "images/ufo_red.png"

#define INTRO_MUSIC "audio/intro.mp3"
#define BG_MUSIC "audio/music.mp3"
#define ALLY_ATTACK "audio/ally.wav"
#define ENEMY_ATTACK "audio/enemy.wav"
#define WIN_MUSIC "audio/win.mp3"
#define GAME_OVER_MUSIC "audio/gameover.mp3"

#define EASY_LEVEL 1
#define MEDIUM_LEVEL 2
#define HARD_LEVEL 3

#define SHIP_SPEED 100

#define Z_BG 0
#define Z_PLANETS 1
#define Z_INDICATORS 2
#define Z_SHIPS 3
#define Z_GUI 4

#endif // __UTILS_H__
