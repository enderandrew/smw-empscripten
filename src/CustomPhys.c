#include "CustomPhys.h"

/* Phys Funcs */
/*  PHYS_DoCelDash(xAdd, yAdd);
    Adds to (or sets) Mario's speed based on given args.
xAdd,yAdd   ;   Values to add to Mario's speed.
doSet       ;   Sets Mario's speed instead of adding.
*/
void PHYS_DoCelDash(int8 xAdd, int8 yAdd, int doSet){
    //printf("PHYS_DoCelDash(): Doing dash.\n");
    int moveHort = (CON_HOLD_LEFT >= 1) - (CON_HOLD_RIGHT >= 1);
    int moveVert = (CON_HOLD_DOWN >= 1) - (CON_HOLD_UP >= 1);

    //printf("PHYS_DoCelDash ; Moving towards: (%u, %u)\n.", moveHort, moveVert);

    if(doSet){
        player_xspeed = moveHort * xAdd;
        player_yspeed = moveVert * yAdd;
    }else{
        player_xspeed += moveHort * xAdd;
        player_yspeed += moveVert * yAdd;
    }
}

/*  PHYS_DoDoubleJump(yAdd, doSpin);
    Sets Mario's yspeed to yAdd.
yAdd    ;   What to add to Mario's yspeed.
doSpin  ;   Whether or not to turn on Mario's spin jump.
*/
void PHYS_DoDoubleJump(int8 yAdd, int doSpin){
    printf("PHYS_DoDoubleJump: Doing jump.\n");
    if(doSpin)
        player_spin_jump_flag = 1;
    player_yspeed = -yAdd;
}

/*  PHYS_StepSmashPit();
    Records player's grounded position every second.
    Restores player to that position if they fall in a pit.
*/
void PHYS_StepSmashPit(){
    //printf("player_on_screen_pos_y: %u.\n", player_on_screen_pos_y);
    if(player_on_screen_pos_y > 240 && player_on_screen_pos_y < 245){
        player_xspeed = player_yspeed = 0;
        player_xpos = lastGroundx;
        player_ypos = lastGroundy;
    }

    if(counter_global_frames % 60 == 0 && !player_in_air_flag){
        //printf("PHYS_StepSmashPit(): Tracking player pos.\n");
        lastGroundx = player_xpos;
        lastGroundy = player_ypos;
    }
}

/* Challenge Funcs */
/*  CHAL_InitCoinChallenge(coins, time, disable);
    Inits the coin challenge with the specified parameters.
coins   ;   The amount of coins started with per-level.
time    ;   The amount of time before coins are drained in seconds.
disable ;   Disables the level timer.
*/
void CHAL_InitCoinChallenge(uint8 coins, int time, int disable){
    cc_startingCoins = coins;
    cc_coinTimer_Max = cc_coinTimer_Cur = time * 60;
    cc_disableLevelTimer = disable;
}

/*  CHAL_StepCoinChallenge();
    Drains coins based on timer and gamestate.
*/
void CHAL_StepCoinChallenge(){
    if(misc_game_mode == gm_Level){
        //printf("CHAL_StepCoinChallenge(): Coin Timer: %i.\n", cc_coinTimer_Cur);
        cc_coinTimer_Cur--;
        if(cc_coinTimer_Cur <= 0 && player_current_coin_count > 0){
            //printf("CHAL_StepCoinChallenge(): Stealing Coin.\n");
            player_current_coin_count--;
            cc_coinTimer_Cur = cc_coinTimer_Max;
            
        }else if(player_current_coin_count == 0){
            DamagePlayer_Kill();
        }
    }else{
        player_current_coin_count = cc_startingCoins;
    }
}

/*  CHAL_SetTimerVars();
    Sets the given src and des rect based on the given params.
    Intended to be used in CHAL_InitEscape().
src,des ;   Texture rects.
digit   ;   Which digit this texture is for.
pos     ;   Which place the digit should be placed.
*/
void CHAL_SetTimerVars(SDL_Rect* src, SDL_Rect* des, int digit, int pos){
    src->x = (digit % 10) * 30;
    des->x = (pos * (int)ec_digWid / 2) + 25;
    src->y = 0;
    des->y = 45;
    src->w = 30;
    des->w = (int)ec_digHgt / 1.25;
    src->h = 43;
    des->h = (int)ec_digHgt / 1.25;
}

/*  CHAL_InitEscape(time,hortRange);
    Starts the escape sequence.
time        ;   Escape timer in seconds.
hortRange   ;   How far player has to be from the left side of the level to finish the escape.
*/
void CHAL_InitEscape(int time, uint8 hortRange){
    ec_escapeTimer = time * 60;
    ec_finishHortRange = hortRange;
    ec_startedEscape = 1;
    ec_finishedEscape = 0;
    printf("CHAL_InitEscape(): Starting escape.\n");

    // Load texture-related vars.
    ec_digWid = 30 / 2;
    ec_digHgt = 43 / 2;
    ec_tvWid = 278;
    ec_tvHgt = 268;
    ec_textureTimer = IMG_LoadTexture(g_renderer, "./assets/custom/Textures/PizzaTower/PTTV.png");
    ec_textureTimerDigits = IMG_LoadTexture(g_renderer, "./assets/custom/Textures/PizzaTower/PTUI.png");
    printf("%s.\n", IMG_GetError());
}

/*  CHAL_StepEscape();
    Counts down the escape timer, checks if Player is finished.
*/
void CHAL_StepEscape(){
    ec_escapeTimer--;
    if(ec_escapeTimer <= 0){
        /*if(!spawnedPizzaFace)
            CHAL_PizzaInit(player_xpos, player_ypos);*/
        // Temp
        DamagePlayer_Kill();
        CHAL_EndEscape();
    }
    //printf("CHAL_StepEscape(): Time left %u.\n", (int)ec_escapeTimer / 60);

    if(player_xpos <= ec_finishHortRange){
        //EndLevel_Tape(0);
        CHAL_EndEscape();
        PlayerState00_LevelFinished(11,2);
        ec_finishedEscape = 1;
    }
}

/*  CHAL_DrawEscape();
    Draws the escape timer and Pizza Head.
*/
void CHAL_DrawEscape(){
    // TV
    SDL_Rect sTV, dTV;
    sTV.x = sTV.y = dTV.x = dTV.y = 0;
    sTV.w = ec_tvWid;
    sTV.h = ec_tvHgt;
    dTV.w = (int)ec_tvWid/3;
    dTV.h = (int)ec_tvHgt/3;
    SDL_RenderCopy(g_renderer, ec_textureTimer, &sTV, &dTV);

    // Digits
    SDL_Rect sDig1, dDig1, sDig2, dDig2, sDig3, dDig3;
    CHAL_SetTimerVars(&sDig1, &dDig1, ((int)ec_escapeTimer/60)%1000 / 100, 0);  // 100s
    CHAL_SetTimerVars(&sDig2, &dDig2, ((int)ec_escapeTimer/60)%100 / 10, 1);    // 10s
    CHAL_SetTimerVars(&sDig3, &dDig3, ((int)ec_escapeTimer/60)%10, 2);          // 1s

    SDL_RenderCopy(g_renderer, ec_textureTimerDigits, &sDig1, &dDig1);
    SDL_RenderCopy(g_renderer, ec_textureTimerDigits, &sDig2, &dDig2);
    SDL_RenderCopy(g_renderer, ec_textureTimerDigits, &sDig3, &dDig3);
}

/*  CHAL_EndEscape();
    Ends the escape seq.
*/
void CHAL_EndEscape(){
    ec_startedEscape = 0;
    ec_finishHortRange = 0;
    ec_escapeTimer = 0;
    ec_spawnedPizzaFace = 0;
}

/*  CHAL_PizzaInit(x,y);
    Inits Pizza Face at the specifed coords.
x,y     ;   You know what these are.
*/
void CHAL_PizzaInit(uint8 x, uint8 y){}

/*  CHAL_PizzaStep();
    Moves Pizza Face.
*/
void CHAL_PizzaStep();