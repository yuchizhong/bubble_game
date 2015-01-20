//
//  bubble.cpp
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#include "game.h"
#include "mainScene.h"
#include "overScene.h"

game *_manager = NULL;

game* game::sharedGameManager() {
    if (_manager == NULL) {
        _manager = new game();
        _manager->status = INITED;
        _manager->score = 0;
        _manager->error_count = 0;
        _manager->correct_count = 0;
        _manager->time_passed = 0;
        _manager->difficulty_count = 0;
    }
    return _manager;
}

void game::start() {
    sharedGameManager()->status = INGAME;
    sharedGameManager()->score = 0;
    sharedGameManager()->error_count = 0;
    sharedGameManager()->correct_count = 0;
    sharedGameManager()->time_passed = 0;
    sharedGameManager()->difficulty_count = 0;
    
    auto director = cocos2d::Director::getInstance();
    auto scene = mainScene::createScene();
    director->replaceScene(scene);
}

void game::pause() {
    sharedGameManager()->status = PAUSED;
}

void game::resume() {
    sharedGameManager()->status = INGAME;
}

void game::correct(int scoreGet) {
    if (sharedGameManager()->status == INGAME) {
        sharedGameManager()->correct_count++;
        sharedGameManager()->score += scoreGet;
        sharedGameManager()->difficulty_count++;
    }
}

bool game::overOnError() {
    if (sharedGameManager()->error_count >= MAXERROR) {
        gameOver();
        return true;
    }
    return false;
}

void game::getError() {
    if (sharedGameManager()->status == INGAME) {
        sharedGameManager()->error_count++;
        if (sharedGameManager()->error_count > MAXERROR) {
            sharedGameManager()->error_count = MAXERROR;
        }
    }
}

void game::gameOver() {
    //save highest score
    if (getHighScore() < score)
        setHighScore(score);
    
    sharedGameManager()->status = GAMEOVER;
    auto director = cocos2d::Director::getInstance();
    auto scene = overScene::createScene();
    auto trans = cocos2d::TransitionMoveInR::create(0.75f, scene);
    director->replaceScene(trans);
}

void game::timePassed(float dt) {
    if (sharedGameManager()->status == INGAME) {
        sharedGameManager()->time_passed += dt;
    }
}

void game::setHalo(int withHalo) {
    halo = withHalo;
}

void game::resetDifficultyCount() {
    difficulty_count = 0;
}

void game::setHighScore(long high) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey("highScore", (int)high);
}

long game::getHighScore() {
    long high = cocos2d::UserDefault::getInstance()->getIntegerForKey("highScore", 0);
    return high;
}
