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
static string explosion_prefix[3] = {"BLUE", "粉色爆炸", "黄色爆炸"};

static int scores[3] = {5, 20, 50};

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
        
        //initiate explosion animation
        auto animation = cocos2d::Animation::create();
        for(int i = EXPLOSION_BEGIN_FRAME; i <= EXPLOSION_END_FRAME; i++) {
            char expfilename[30] = {0};
            sprintf(expfilename, "%s_%05d.png", explosion_prefix[type].c_str(), i);
            animation->addSpriteFrameWithFile(string(expfilename));
        }
        animation->setDelayPerUnit(EXPLOSION_FRAME_DELAY);
        animation->setRestoreOriginalFrame(false);
        animation->setLoops(1);
        auto action = cocos2d::Animate::create(animation);
        b->explosion_animation = action;
        
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void bubble::setRotation(float init_ro, float ro_rate) {
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
    
    this->setPosition(this->current_x, this->current_y);
    this->setScale(this->current_r, this->current_r);
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
}

void bubble::onDeath(bool punish) {
    //remove bubble from layer at a count down
    scheduleOnce(schedule_selector(bubble::removeFromLayer), EXPLOSION_REMOVE_DELAY);
    runAction(cocos2d::Sequence::create(explosion_animation, NULL));
    //explosion textures not available
    container->removeChildByTag(getTag());
    if (punish)
        game::sharedGameManager()->overOnError();
}

void bubble::onTouch() {
    container->removeChildByTag(getTag());
    int scoreToEarn = scores[age];
    game::sharedGameManager()->correct(scoreToEarn);
}
