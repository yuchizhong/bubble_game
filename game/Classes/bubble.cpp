//
//  bubble.cpp
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#include "bubble.h"
#include "game.h"

static string bubble_name[3] = {"BLUE", "PINK", "YELLOW"};
static string explosion_prefix[3] = {"BLUE", "PINK", "YELLOW"};

static int scores[MAX_AGE + 1] = {10, 20, 50};

static int getRand(int start, int end) {
    float i = CCRANDOM_0_1()*(end-start+1)+start;  //产生一个从start到end间的随机数
    return (int)i;
}

void bubble::startCache() {
    for (int type = 0; type < 3; type++) {
        auto animation = cocos2d::Animation::create();
        for(int i = EXPLOSION_BEGIN_FRAME; i <= EXPLOSION_END_FRAME; i++) {
            char expfilename[30] = {0};
            sprintf(expfilename, "%s_%05d.png", explosion_prefix[type].c_str(), i);
            animation->addSpriteFrameWithFile(string(expfilename));
        }
        animation->setDelayPerUnit(EXPLOSION_FRAME_DELAY);
        animation->setRestoreOriginalFrame(false);
        animation->setLoops(1);
        cocos2d::AnimationCache::getInstance()->addAnimation(animation, explosion_prefix[type]);
    }
}

bubble* bubble::create(int type, float x, float y, float startingRadius, float rate, float dx, float dy) {
    bubble *b = new (std::nothrow) bubble();
    char filename[30] = {0};
    sprintf(filename, "%s-PP%d.png", bubble_name[type].c_str(), 0);
    string texture = string(filename);
    if (b && b->initWithFile(texture))
    {
        b->autorelease();
        b->bubble_type = type;
        
        b->current_x = x;
        b->current_y = y;
        b->current_r = startingRadius;
        b->r_rate = rate;
        b->delta_x = dx;
        b->delta_y = dy;
        b->timePassed = 0.0f;
        b->age = 0;
        b->textureAge = 0;
        
        b->setPosition(b->current_x, b->current_y);
        b->setScale(b->current_r, b->current_r);
        
        b->setMyRotation((float)getRand(0, 360), 5.0 - (float)getRand(0, 10));
        
        //start action
        b->setOpacity(0);
        cocos2d::FadeIn *fin = cocos2d::FadeIn::create(0.3f);
        b->runAction(fin);
        
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void bubble::setMyRotation(float init_ro, float ro_rate) {
    current_angle = init_ro;
    angle_rate = ro_rate;
    this->cocos2d::Sprite::setRotation(current_angle);
}

void bubble::update(float dt) {
    current_x += delta_x * dt;
    current_y += delta_y * dt;
    current_r *= 1.0 + this->r_rate * dt;
    current_angle += angle_rate * dt;
    timePassed += dt;
    
    //update age
    age = timePassed / 2.0;
    if (age > MAX_AGE)
        age = MAX_AGE;
    
    this->cocos2d::Sprite::setPosition(this->current_x, this->current_y);
    this->cocos2d::Sprite::setScale(this->current_r, this->current_r);
    this->cocos2d::Sprite::setRotation(current_angle);
    
    //update texture
    if (textureAge < age) {
        char filename[30] = {0};
        sprintf(filename, "%s-PP%d.png", bubble_name[this->bubble_type].c_str(), age);
        this->setTexture(string(filename));
        textureAge = age;
    }
}

void bubble::removeFromLayer(float dt) {
    container->removeChildByTag(getTag());
    
    if (this->punishUser)
        game::sharedGameManager()->overOnError();
}

void bubble::onDeath(bool punish) {
    this->punishUser = punish;
    
    if (punish)
        game::sharedGameManager()->getError();
    
    //remove bubble from layer at a count down
    scheduleOnce(schedule_selector(bubble::removeFromLayer), EXPLOSION_REMOVE_DELAY);
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(explosion_prefix[this->bubble_type]);
    auto action = cocos2d::Animate::create(animation);
    runAction(cocos2d::Sequence::create(action, NULL));
}

void bubble::onTouch() {
    int scoreToEarn = scores[age];
    CCLOG("GET %ld score", scoreToEarn);
    game::sharedGameManager()->correct(scoreToEarn);
    
    //remove bubble from layer at a count down
    scheduleOnce(schedule_selector(bubble::removeFromLayer), EXPLOSION_REMOVE_DELAY);
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation(explosion_prefix[this->bubble_type]);
    auto action = cocos2d::Animate::create(animation);
    runAction(cocos2d::Sequence::create(action, NULL));
}