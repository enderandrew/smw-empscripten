#include "CustomSound.h"

/* Music Funcs */
/*  MUS_LoadBasedContext();
    Loads a music file based on certain gamestates.
    Requires all songs to be defined first.
    Returns 0 if no suitable song is found.
*/
int MUS_LoadBasedContext(){
    if(io_music_ch1 == MUSID_STOP){
        Mix_PauseMusic();
        printf("MUS_LoadBasedContext(): Stopping current track.\n");
        return 1;
    }

    if(isInterrupt){
        printf("MUS_LoadBasedContext(): Loading interrupted music: %s\n", curTrackPath_Back);
        MUS_LoadFile(curTrackPath_Back, curMusicPos_Back, curMusicEndPoint_Back, curMusicLoopPoint_Back, curMusicDoLoop_Back, 0);
        return 1;
    }

    //printf("misc_game_mode: %u.\n", misc_game_mode);

    switch(misc_game_mode){
        case gm_OverworldFadeIn:
            switch(ow_players_map[0]){
                case wm_World1:
                    return MUS_Load("./assets/custom/Overworld/1.txt", 0);
                case wm_World2:
                    return MUS_Load("./assets/custom/Overworld/2.txt", 0);
                case wm_World3:
                    return MUS_Load("./assets/custom/Overworld/3.txt", 0);
                case wm_World5:
                    return MUS_Load("./assets/custom/Overworld/5.txt", 0);
                case wm_World7:
                    return MUS_Load("./assets/custom/Overworld/7.txt", 0);
                case wm_Special:
                    return MUS_Load("./assets/custom/Overworld/Special.txt", 0);
                case wm_Star:
                    return MUS_Load("./assets/custom/Overworld/Star.txt", 0);
            }
            break;
        case gm_PrepareLevel:   /*  TODO: Make this bit based on level IDs. 
                                    Also add in the Func. Music.
                                */
            switch(io_music_ch1){
                case MUSID_ATHLETIC:
                    return MUS_Load("./assets/custom/Level/Athletic.txt", 0);
                case MUSID_OVERWORLD:
                    return MUS_Load("./assets/custom/Level/Overworld.txt", 0);
                case MUSID_SWIMMING:
                    return MUS_Load("./assets/custom/Level/Swimming.txt", 0);
                case MUSID_UNDERGROUND:
                    return MUS_Load("./assets/custom/Level/Underground.txt", 0);
                case MUSID_HAUNTED:
                    return MUS_Load("./assets/custom/Level/Haunted.txt", 0);
                case MUSID_CASTLE:
                    return MUS_Load("./assets/custom/Level/Castle.txt", 0);
                case MUSID_PALACE:
                    return MUS_Load("./assets/custom/Level/Palace.txt", 0);
                case MUSID_BOSS:
                    return MUS_Load("./assets/custom/Level/Boss.txt", 0);
            }
            break;
        case gm_Level:
            switch(io_music_ch1){
                case MUSID_PSWITCH:
                    return MUS_Load("./assets/custom/Gameplay/PSwitch.txt", 1);
            }
            break;
    }
    return 0;
}

/*  MUS_Load(filePath, doInterrupt);
    Loads a music file based on given params.
    Returns 0 if file isn't found.
filePath    ;   Path to music file.
doInerrupt  ;   Whether or not the loaded track should interrupt the current track.
*/
int MUS_Load(const char* filePath, int doInterrupt){
    char* curLine = malloc(256);
    char* copyLine = malloc(256);
    
    // Read in file.
    FILE* readFile = fopen(filePath, "r");
    if(readFile == NULL){
        printf("MUS_Load() Err: Can't find file at %s.\n", filePath);
        return 0;
    }
    
    // Path to MP3 file.
    fgets(curLine, 256, readFile);
    strncpy(copyLine, curLine, strlen(curLine) - 1);
    copyLine[strlen(curLine) - 1] = NULL;
    gMusic_Playing = Mix_LoadMUS(copyLine);
    if(gMusic_Playing == NULL){
        printf("MUS_Load() Err: Mix_LoadMUS() Err: %s.\n", Mix_GetError());
        return 0;
    }

    // Store interrupt data if applicable.
    if(doInterrupt && USE_MUSICVAR_BACK){
        //printf("curMusicPos: %u.\n", curMusicPos);
        isInterrupt = 1;
        curTrackPath_Back = curTrackPath;
        curMusicPos_Back = curMusicPos;
        curMusicLoopPoint_Back = curMusicLoopPoint;
        curMusicEndPoint_Back = curMusicEndPoint;
        curMusicDoLoop_Back = curMusicDoLoop;
    }else{
        isInterrupt = 0;
    }
    curTrackPath = copyLine;

    // Tracking vars.
    // Line 1: Start Pos
    // Line 2: End Pos
    // Line 3: Loop Pos
    // Line 4: Do Loop
    fgets(curLine, 256, readFile);
    curMusicPos = atoi(curLine) / 1000;
    fgets(curLine, 256, readFile);
    curMusicEndPoint = atoi(curLine);
    fgets(curLine, 256, readFile);
    curMusicLoopPoint = atoi(curLine);
    fgets(curLine, 256, readFile);
    curMusicDoLoop = atoi(curLine);

    //printf("curMusicPos: %u.\n", curMusicPos);

    fclose(readFile);
    free(curLine);
    free(copyLine);

    // Play music.
    Mix_PlayMusic(gMusic_Playing, 0);
    Mix_SetMusicPosition((double)curMusicPos / 1000);
    Mix_ResumeMusic();

    // Everything read correctly! (Yay)
    return 1;
}

/*  MUS_LoadFile(path, start, end, loop, doLoop);
    Loads a music file based on the given params.
path            ;   Path to MP3.
start,end,loop  ;   Timing points.
doLoop          ;   Whether or not the track should loop.
doInterrupt     ;   Whether or not the track should interrupt the current track.
*/
int MUS_LoadFile(const char* path, double* start, double* end, double* loop, int* doLoop, int* doInterrupt){
    // Load MP3.
    gMusic_Playing = Mix_LoadMUS(path);
    if(gMusic_Playing == NULL){
        printf("MUS_Load() Err: Mix_LoadMUS() Err: %s.\n", Mix_GetError());
        return 0;
    }

    // Store interrupt data if applicable.
    if(doInterrupt && USE_MUSICVAR_BACK){
        isInterrupt = 1;
        curTrackPath_Back = curTrackPath;
        curMusicPos_Back = curMusicPos;
        curMusicLoopPoint_Back = curMusicLoopPoint;
        curMusicEndPoint_Back = curMusicEndPoint;
        curMusicDoLoop_Back = curMusicDoLoop;
    }else{
        isInterrupt = 0;
    }
    curTrackPath = path;

    // Tracking vars.
    curMusicPos = start;
    curMusicLoopPoint = loop;
    curMusicEndPoint = end;
    curMusicDoLoop = doLoop;

    // Play music.
    Mix_PlayMusic(gMusic_Playing, 0);
    Mix_SetMusicPosition((double)curMusicPos / 1000);
    Mix_ResumeMusic();

    return 1;
}

/*  MUS_Step();
    Tracks current music.
*/
void MUS_Step(){
    if(gMusic_Playing != NULL && !Mix_PausedMusic()){
        double temp = Mix_GetMusicPosition(gMusic_Playing) * 1000;
        curMusicPos = (double*)temp;
    }
    
    //printf("Stuff: %u / %u / %u.\n", curMusicEndPoint, curMusicLoopPoint, curMusicPos);
    //printf("counter_global_frames: %u.\n", counter_global_frames);

    if(curMusicPos >= curMusicEndPoint && curMusicDoLoop)
        Mix_SetMusicPosition((double)curMusicLoopPoint / 1000);
}

/* Sound Funcs */
/*  SND_Load_Minor(target, path);
    Loads a WAV file into the specified chunk from the specified path.
    Loaded files CANNOT have a newline at the end.
    Please god do not put a newline at the end.
    Meant to be used with SND_Load().
    Returns 0 on failure.
target  ;   Target Mix_Chunk.
path    ;   Path to file.
*/
int SND_Load_Minor(Mix_Chunk* target, const char* path){
    char* curLine = malloc(256);
    
    // Read in file.
    FILE* readFile = fopen(path, "r");
    if(readFile == NULL){
        printf("SND_Load_Minor() Err: Can't find file at %s.\n", path);
        return 0;
    }

    // Load WAV file.
    fgets(curLine, 256, readFile);
    printf("SND_Load_Minor(): Attempting to load %s.\n", curLine);
    target = Mix_LoadWAV(curLine);
    if(target == NULL){
        printf("SND_Load_Minor() Err: Mix_LoadWAV() Err: %s.\n", Mix_GetError());
        return 0;
    }

    fclose(readFile);
    free(curLine);

    printf("SND_Load_Minor(): Success!\n");
    return 1;
}

/*  SND_Load();
    Loads all external sound files.
*/
void SND_Load(){
    printf("Loading Custom SFX...\n");
    SND_Load_Minor(gSound_Default, "./assets/custom/SFX/default.txt");

    SND_Load_Minor(gSound_Pipe, "./assets/custom/SFX/Levels/pipe.txt");
    SND_Load_Minor(gSound_CheckPoint, "./assets/custom/SFX/Levels/checkpoint.txt");
    SND_Load_Minor(gSound_SwitchBlock, "./assets/custom/SFX/Levels/switch.txt");
    SND_Load_Minor(gSound_ItemPastGoal, "./assets/custom/SFX/Levels/itempastgoal.txt");
    SND_Load_Minor(gSound_YoshiCoin, "./assets/custom/SFX/Levels/yoshicoin.txt");
    SND_Load_Minor(gSound_Grinder, "./assets/custom/SFX/Levels/grinder.txt");
    SND_Load_Minor(gSound_Spring, "./assets/custom/SFX/Levels/spring.txt");
    SND_Load_Minor(gSound_Door, "./assets/custom/SFX/Levels/door.txt");
    SND_Load_Minor(gSound_MsgBox, "./assets/custom/SFX/Levels/msgbox.txt");

    SND_Load_Minor(gSound_YoshiGulp, "./assets/custom/SFX/Yoshi/yoshigulp.txt");
    SND_Load_Minor(gSound_YoshiSpit, "./assets/custom/SFX/Yoshi/yoshispit.txt");
    SND_Load_Minor(gSound_YoshiHatch, "./assets/custom/SFX/Yoshi/yoshihatch.txt");
    SND_Load_Minor(gSound_YoshiLose, "./assets/custom/SFX/Yoshi/yoshilose.txt");
    SND_Load_Minor(gSound_YoshiFire, "./assets/custom/SFX/Yoshi/yoshifire.txt");
    SND_Load_Minor(gSound_YoshiMount, "./assets/custom/SFX/Yoshi/yoshimount.txt");
    SND_Load_Minor(gSound_YoshiTongue, "./assets/custom/SFX/Yoshi/yoshitongue.txt");
    SND_Load_Minor(gSound_YoshiStomp, "./assets/custom/SFX/Yoshi/yoshistomp.txt");

    SND_Load_Minor(gSound_Fly, "./assets/custom/SFX/Mario/fly.txt");
    SND_Load_Minor(gSound_FlyHurt, "./assets/custom/SFX/Mario/flyhurt.txt");
    SND_Load_Minor(gSound_HitHead, "./assets/custom/SFX/Mario/hithead.txt");
    SND_Load_Minor(gSound_SpinJumpHop, "./assets/custom/SFX/Mario/spinjumphop.txt");
    SND_Load_Minor(gSound_SpinJumpKill, "./assets/custom/SFX/Mario/spinjumpkill.txt");
    SND_Load_Minor(gSound_SpinJump, "./assets/custom/SFX/Mario/spinjump.txt");
    SND_Load_Minor(gSound_Kick, "./assets/custom/SFX/Mario/kick.txt");
    SND_Load_Minor(gSound_PowerUp, "./assets/custom/SFX/Mario/powerup.txt");
    SND_Load_Minor(gSound_GetCape, "./assets/custom/SFX/Mario/getcape.txt");
    SND_Load_Minor(gSound_Swim, "./assets/custom/SFX/Mario/swim.txt");
    SND_Load_Minor(gSound_GetPBalloon, "./assets/custom/SFX/Mario/getpballoon.txt");
    SND_Load_Minor(gSound_Jump, "./assets/custom/SFX/Mario/jump.txt");
    SND_Load_Minor(gSound_Fireball, "./assets/custom/SFX/Mario/fireball.txt");
    SND_Load_Minor(gSound_ItemInReserve, "./assets/custom/SFX/Mario/iteminreserve.txt");
    SND_Load_Minor(gSound_ItemOutReserve, "./assets/custom/SFX/Mario/itemoutreserve.txt");
    SND_Load_Minor(gSound_LRScroll, "./assets/custom/SFX/Mario/lrscroll.txt");

    SND_Load_Minor(gSound_KoopaShrink, "./assets/custom/SFX/Boss/koopashrink.txt");
    SND_Load_Minor(gSound_LemmyWendyFall, "./assets/custom/SFX/Boss/lemmywendyfall.txt");
    SND_Load_Minor(gSound_LemmyWendyLava, "./assets/custom/SFX/Boss/lemmywendylava.txt");
    SND_Load_Minor(gSound_PeachHelp, "./assets/custom/SFX/Boss/peachhelp.txt");

    SND_Load_Minor(gSound_TileReveal, "./assets/custom/SFX/Overworld/tilereveal.txt");
    SND_Load_Minor(gSound_CastleCollapse, "./assets/custom/SFX/Overworld/castlecollapse.txt");
    SND_Load_Minor(gSound_BlockEjection, "./assets/custom/SFX/Overworld/blockejection.txt");
    SND_Load_Minor(gSound_ToLevelTile, "./assets/custom/SFX/Overworld/toleveltile.txt");

    SND_Load_Minor(gSound_FireworkWhistle, "./assets/custom/SFX/Ending/fireworkwhistle.txt");
    SND_Load_Minor(gSound_FireworkBang, "./assets/custom/SFX/Ending/fireworkbang.txt");

    SND_Load_Minor(gSound_Pause, "./assets/custom/SFX/Misc/drybones.txt");
    SND_Load_Minor(gSound_Magic, "./assets/custom/SFX/Misc/magic.txt");
    SND_Load_Minor(gSound_Pause, "./assets/custom/SFX/Misc/pause.txt");
    SND_Load_Minor(gSound_Unpause, "./assets/custom/SFX/Misc/unpause.txt");
    SND_Load_Minor(gSound_Stomp1, "./assets/custom/SFX/Misc/stomp1.txt");
    SND_Load_Minor(gSound_Stomp2, "./assets/custom/SFX/Misc/stomp2.txt");
    SND_Load_Minor(gSound_Stomp3, "./assets/custom/SFX/Misc/stomp3.txt");
    SND_Load_Minor(gSound_Stomp4, "./assets/custom/SFX/Misc/stomp4.txt");
    SND_Load_Minor(gSound_Stomp5, "./assets/custom/SFX/Misc/stomp5.txt");
    SND_Load_Minor(gSound_Stomp6, "./assets/custom/SFX/Misc/stomp6.txt");
    SND_Load_Minor(gSound_Stomp7, "./assets/custom/SFX/Misc/stomp7.txt");
    SND_Load_Minor(gSound_TimeLow, "./assets/custom/SFX/Misc/timelow.txt");
    SND_Load_Minor(gSound_World7Appears, "./assets/custom/SFX/Misc/world7appears.txt");
    SND_Load_Minor(gSound_BlarggRoar, "./assets/custom/SFX/Misc/blarggroar.txt");
    SND_Load_Minor(gSound_Coin, "./assets/custom/SFX/Misc/coin.txt");
    SND_Load_Minor(gSound_PowerBlock, "./assets/custom/SFX/Misc/powerblock.txt");
    SND_Load_Minor(gSound_VineBlock, "./assets/custom/SFX/Misc/vineblock.txt");
    SND_Load_Minor(gSound_1UP, "./assets/custom/SFX/Misc/1up.txt");
    SND_Load_Minor(gSound_DestroyedBlock, "./assets/custom/SFX/Misc/destroyedblock.txt");
    SND_Load_Minor(gSound_BulletBill, "./assets/custom/SFX/Misc/bulletbill.txt");
    SND_Load_Minor(gSound_DrumRollStart, "./assets/custom/SFX/Misc/drumrollstart.txt");
    SND_Load_Minor(gSound_DrumRollEnd, "./assets/custom/SFX/Misc/drumrollend.txt");
    SND_Load_Minor(gSound_Thunder, "./assets/custom/SFX/Misc/thunder.txt");
    SND_Load_Minor(gSound_ChuckClap, "./assets/custom/SFX/Misc/chuckclap.txt");
    SND_Load_Minor(gSound_BombBlow, "./assets/custom/SFX/Misc/bombblow.txt");
    SND_Load_Minor(gSound_BombFuse, "./assets/custom/SFX/Misc/bombfuse.txt");
    SND_Load_Minor(gSound_Whistle, "./assets/custom/SFX/Misc/whistle.txt");
    SND_Load_Minor(gSound_PSwitchOver, "./assets/custom/SFX/Misc/pswitchover.txt");
    SND_Load_Minor(gSound_Swooper, "./assets/custom/SFX/Misc/swooper.txt");
    SND_Load_Minor(gSound_Podoboo, "./assets/custom/SFX/Misc/podoboo.txt");
    SND_Load_Minor(gSound_Stunned, "./assets/custom/SFX/Misc/stunned.txt");
    SND_Load_Minor(gSound_Correct, "./assets/custom/SFX/Misc/correct.txt");
    SND_Load_Minor(gSound_Wrong, "./assets/custom/SFX/Misc/wrong.txt");
}

/* TODO: The next three methods suck and need to be redone. */

/*  SND_Step_Ch1();
    Tracks and plays certain sounds based on io_sound_ch1.
    Requires all sounds to be defined first. (Call SND_Load().)
    Returns -1 if Mix_PlayChannel() fails or no suitable file was found.
*/
int SND_Step_Ch1(){
    // Mix_PlayChannel(1,NULL,0);
    switch(io_sound_ch1){
        case 0:
            return 0;
        case SNDID_HITHEAD:
            return Mix_PlayChannel(1,gSound_HitHead,0);
        case SNDID_SPINJUMPHOP:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_KICK:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_PIPE:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_CHECKPOINT:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_YOSHIGULP:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_DRYBONES:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_SPINJUMPKILL:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_FLY:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_POWERUP:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_SWITCHBLOCK:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_ITEMPASTGOAL:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_GETCAPE:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_SWIM:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_FLYHURT:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_MAGIC:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_PAUSE:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_UNPAUSE:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP1:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP2:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP3:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP4:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP5:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP6:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_STOMP7:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_YOSHICOIN:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_TIMELOW:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_GETPBALLOON:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_KOOPASHRINK:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_YOSHISPIT:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_WORLD7APPEARS:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_LEMMYWENDYFALL:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_BLARGGROAR:
            return Mix_PlayChannel(1,NULL,0);
        case SNDID_PEACHHELP:
            return Mix_PlayChannel(1,NULL,0);    
    }
    
    printf("SND_Step_Ch1(): No suitable sound found.\n");
    return -1;
}

/*  SND_Step_Ch2();
    Tracks and plays certain sounds based on io_sound_ch2.
    Requires all sounds to be defined first. (Call SND_Load().)
    Returns -1 if Mix_PlayChannel() fails or no suitable file was found.
*/
int SND_Step_Ch2(){
    // Mix_PlayChannel(2,NULL,0);
    switch(io_sound_ch2){
        case 0:
            return 0;
        case SNDID_JUMP:
            return Mix_PlayChannel(2,gSound_Jump,0);
        case SNDID_GRINDER:
            return Mix_PlayChannel(2,gSound_Grinder,0);
    }

    printf("SND_Step_Ch2(): No suitable sound found.\n");
    return -1;
}

/*  SND_Step_Ch3();
    Tracks and plays certain sounds based on io_sound_ch3.
    Requires all sounds to be defined first. (Call SND_Load().)
    Returns -1 if Mix_PlayChannel() fails or no suitable file was found.
*/
int SND_Step_Ch3(){
    // Mix_PlayChannel(3,NULL,0);
    switch(io_sound_ch3){
        case 0:
            return 0;
        case SNDID_COIN:
            return Mix_PlayChannel(3,gSound_Coin,0);
        case SNDID_POWERBLOCK:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_VINEBLOCK:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_SPINJUMP:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_1UP:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_FIREBALL:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_DESTROYEDBLOCK:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_SPRING:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_BULLETBILL:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHIHATCH:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_ITEMINRESERVE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_ITEMOUTRESERVE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_LRSCROLL:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_DOOR:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_DRUMROLLSTART:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_DRUMROLLEND:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHILOSE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_TILEREVEAL:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_CASTLECOLLAPSE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHIFIRE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_THUNDER:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_CHUCKCLAP:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_BOMBBLOW:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_BOMBFUSE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_BLOCKEJECTION:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_WHISTLE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHIMOUNT:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_LEMMYWENDYLAVA:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHITONGUE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_MSGBOX:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_TOLEVELTILE:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_PSWITCHOVER:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_YOSHISTOMP:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_SWOOPER:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_PODOBOO:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_STUNNED:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_CORRECT:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_WRONG:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_FIREWORKBANG:
            return Mix_PlayChannel(3,NULL,0);
        case SNDID_FIREWORKWHISTLE:
            return Mix_PlayChannel(3,NULL,0);
    }

    printf("SND_Step_Ch3(): No suitable sound found.\n");
    return -1;
}