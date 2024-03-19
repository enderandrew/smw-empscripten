#ifndef CUSTOMPHYS.h
#define CUSTOMPHYS.h

#include "variables.h"
#include "types.h"
#include "funcs.h"
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

/* SDL Vars */
SDL_Renderer* renderTarget;

/* Smash Pit Vars */
uint16 lastGroundx;// = 0;
uint16 lastGroundy;// = 0;

/* Coin Challenge Vars */
uint8 cc_startingCoins;    // The amount of coins the player starts with per-level.
int cc_coinTimer_Max;      // Time in-between each drain in seconds.
int cc_coinTimer_Cur;      // Time before next coin is drained.
int cc_disableLevelTimer;  // Whether or not to disable the level timer.

/* Escape Challenge Vars */
int ec_finishedEscape;             // Whether or not player has reached the end.
int ec_startedEscape;
uint8 ec_finishHortRange;          // How close Player has to be to the left-most side of the level to complete it after escape.
int ec_escapeTimer;                // Time until un-timely demise in frames.
int* ec_spawnedPizzaFace;           // Whether or not Pizza Face has spawned.
uint8* pizzaFace_x, *pizzaFace_y;   // God do I even want to attempt this?
uint8* pizzaFace_w, *pizzaFace_h;   // Size of pizza face.
SDL_Texture* ec_textureTimer;
SDL_Texture* ec_textureTimerDigits;
int ec_tvWid, ec_tvHgt;
int ec_digWid, ec_digHgt;

/* Phys Funcs */
void PHYS_DoCelDash(int8,int8,int);
void PHYS_DoDoubleJump(int8,int);
void PHYS_StepSmashPit();

/* Challenge Funcs */
void CHAL_InitCoinChallenge(uint8,int,int);
void CHAL_StepCoinChallenge();
void CHAL_InitEscape(int,uint8);
void CHAL_StepEscape();
void CHAL_DrawEscape();
void CHAL_EndEscape();
void CHAL_PizzaInit(uint8,uint8);
void CHAL_PizzaStep();

#endif