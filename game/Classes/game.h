//
//  bubble.h
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#ifndef __game__game__
#define __game__game__

#define INITED      0
#define INGAME      1
#define GAMEOVER    2
#define PAUSED      3

#define MAXERROR 5

#include <string>
#include "cocos2d.h"

using namespace std;

class game {
public:
    int halo;
    bool halo_active;
    double halo_time_passed;
    
    int status;
    long score;
    int error_count;
    int correct_count;
    int difficulty_count;
    double time_passed;
    
    static game* sharedGameManager();
    
    void start();
    void pause();
    void resume();
    void correct(int scoreGet);
    bool overOnError();
    void getError();
    void gameOver();
    void timePassed(float dt);
    void setHalo(int withHalo);
    void resetDifficultyCount();
    
    void activateHalo();
    void deactivateHalo();
    
    void setHighScore(long high);
    long getHighScore();
};

#endif
