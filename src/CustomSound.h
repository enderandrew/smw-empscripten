#ifndef CUSTOMSOUND_H
#define CUSTOMSOUND_H

#include <SDL_mixer.h>
#include "variables.h"
#include <string.h>
#include <stdio.h>

/* Music Vars */
const char* curTrackPath;                   // Path to whatever is currently playing.
double* curMusicPos;                         // Music position in ms.
double* curMusicLoopPoint, *curMusicEndPoint; // Where music should loop and end in ms. 
double* curMusicDoLoop;                      // Whether or not the current song should loop.
int* isInterrupt;                             // Whether or not the current track interrupted a previous track.
Mix_Music* gMusic_Playing;                  // The currently playing track.

// The vars below are used in case something like the P-Switch music intterupts the current track.
// Non-vanilla behavior. These are only used if allowed.
const char* curTrackPath_Back;
double* curMusicPos_Back;
double* curMusicLoopPoint_Back, *curMusicEndPoint_Back;
double* curMusicDoLoop_Back;

/* Sound Vars */
Mix_Chunk* gSound_Default;

Mix_Chunk* gSound_Pipe;
Mix_Chunk* gSound_CheckPoint;
Mix_Chunk* gSound_SwitchBlock;
Mix_Chunk* gSound_ItemPastGoal;
Mix_Chunk* gSound_YoshiCoin;
Mix_Chunk* gSound_Grinder;
Mix_Chunk* gSound_Spring;
Mix_Chunk* gSound_Door;
Mix_Chunk* gSound_MsgBox;

Mix_Chunk* gSound_YoshiGulp;
Mix_Chunk* gSound_YoshiSpit;
Mix_Chunk* gSound_YoshiHatch;
Mix_Chunk* gSound_YoshiLose;
Mix_Chunk* gSound_YoshiFire;
Mix_Chunk* gSound_YoshiMount;
Mix_Chunk* gSound_YoshiTongue;
Mix_Chunk* gSound_YoshiStomp;

Mix_Chunk* gSound_Fly;
Mix_Chunk* gSound_FlyHurt;
Mix_Chunk* gSound_HitHead;
Mix_Chunk* gSound_SpinJumpHop;
Mix_Chunk* gSound_SpinJumpKill;
Mix_Chunk* gSound_SpinJump;
Mix_Chunk* gSound_Kick;
Mix_Chunk* gSound_PowerUp;
Mix_Chunk* gSound_GetCape;
Mix_Chunk* gSound_Swim;
Mix_Chunk* gSound_GetPBalloon;
Mix_Chunk* gSound_Jump;
Mix_Chunk* gSound_Fireball;
Mix_Chunk* gSound_ItemInReserve;
Mix_Chunk* gSound_ItemOutReserve;
Mix_Chunk* gSound_LRScroll;

Mix_Chunk* gSound_KoopaShrink;
Mix_Chunk* gSound_LemmyWendyFall;
Mix_Chunk* gSound_LemmyWendyLava;
Mix_Chunk* gSound_PeachHelp;

Mix_Chunk* gSound_TileReveal;
Mix_Chunk* gSound_CastleCollapse;
Mix_Chunk* gSound_BlockEjection;
Mix_Chunk* gSound_ToLevelTile;

Mix_Chunk* gSound_FireworkWhistle;
Mix_Chunk* gSound_FireworkBang;

Mix_Chunk* gSound_Drybones;
Mix_Chunk* gSound_Magic;
Mix_Chunk* gSound_Pause;
Mix_Chunk* gSound_Unpause;
Mix_Chunk* gSound_Stomp1;
Mix_Chunk* gSound_Stomp2;
Mix_Chunk* gSound_Stomp3;
Mix_Chunk* gSound_Stomp4;
Mix_Chunk* gSound_Stomp5;
Mix_Chunk* gSound_Stomp6;
Mix_Chunk* gSound_Stomp7;
Mix_Chunk* gSound_TimeLow;
Mix_Chunk* gSound_World7Appears;
Mix_Chunk* gSound_BlarggRoar;
Mix_Chunk* gSound_Coin;
Mix_Chunk* gSound_PowerBlock;
Mix_Chunk* gSound_VineBlock;
Mix_Chunk* gSound_1UP;
Mix_Chunk* gSound_DestroyedBlock;
Mix_Chunk* gSound_BulletBill;
Mix_Chunk* gSound_DrumRollStart;
Mix_Chunk* gSound_DrumRollEnd;
Mix_Chunk* gSound_Thunder;
Mix_Chunk* gSound_ChuckClap;
Mix_Chunk* gSound_BombBlow;
Mix_Chunk* gSound_BombFuse;
Mix_Chunk* gSound_Whistle;
Mix_Chunk* gSound_PSwitchOver;
Mix_Chunk* gSound_Swooper;
Mix_Chunk* gSound_Podoboo;
Mix_Chunk* gSound_Stunned;
Mix_Chunk* gSound_Correct;
Mix_Chunk* gSound_Wrong;

/* Funcs */
int MUS_LoadBasedContext();
int MUS_Load(const char*,int);
int MUS_LoadFile(const char*,double*,double*,double*,int*,int*);
void MUS_Step();
void SND_Load();
//int SND_Step();
int SND_Step_Ch1();
int SND_Step_Ch2();
int SND_Step_Ch3();

#endif