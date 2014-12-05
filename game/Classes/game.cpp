//
//  bubble.cpp
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#include "game.h"
#include "mainScene.h"

game *_manager = NULL;

game* game::sharedGameManager() {
    if (_manager == NULL) {
        _manager = new game();
        _manager->status = INITED;
        _manager->score = 0;
        _manager->error_count = 0;
        _manager->correct_count = 0;
        _manager->time_passed = 0;
    }
    return _manager;
}

void game::start() {
    sharedGameManager()->status = INGAME;
    sharedGameManager()->score = 0;
    sharedGameManager()->error_count = 0;
    sharedGameManager()->correct_count = 0;
    sharedGameManager()->time_passed = 0;
    
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
    }
}

bool game::overOnError() {
    if (sharedGameManager()->status == INGAME) {
        sharedGameManager()->error_count++;
        if (sharedGameManager()->error_count >= MAXERROR) {
            gameOver();
            return true;
        }
    }
    return false;
}

void game::gameOver() {
    sharedGameManager()->status = GAMEOVER;
}

void game::timePassed(float dt) {
    if (sharedGameManager()->status == INGAME) {
        sharedGameManager()->time_passed += dt;
    }
}
