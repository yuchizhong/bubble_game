//
//  bubble.cpp
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#include "bubble.h"
#include "game.h"

static string textures[3][3] = {
    {"pp1@2x.png", "pp2@2x.png", "pp3@2x.png"},
    {"pp2@2x.png", "pp3@2x.png", "pp1@2x.png"},
    {"pp3@2x.png", "pp1@2x.png", "pp2@2x.png"}
};

static int scores[3] = {5, 20, 50};

bubble* bubble::create(int type, float x, float y, float startingRadius, float rate, float dx, float dy) {
    bubble *b = new (std::nothrow) bubble();
    string texture = textures[type][0];
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
        for(int i = 0; i < EXPLOSION_NUM_FRAMES; i++) {
            char filename[30] = {0};
            sprintf(filename, "explosion_%d_%d.png", type, i);
            animation->addSpriteFrameWithFile(filename);
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
    age = timePassed / 3.0;
    if (age > 2)
        age = 2;
    
    this->setPosition(this->current_x, this->current_y);
    this->setScale(this->current_r, this->current_r);
    this->cocos2d::Sprite::setRotation(current_angle);
    
    //update texture
    if (textureAge < age) {
        this->setTexture(textures[bubble_type][age]);
        textureAge = age;
    }
}

void bubble::removeFromLayer(float dt) {
    container->removeChildByTag(getTag());
}

void bubble::onDeath(bool punish) {
    //remove bubble from layer at a count down
    float countdown = EXPLOSION_NUM_FRAMES * EXPLOSION_FRAME_DELAY;
    scheduleOnce(schedule_selector(bubble::removeFromLayer), countdown);
    runAction(cocos2d::Sequence::create(explosion_animation, NULL));
    if (punish)
        game::sharedGameManager()->overOnError();
}

void bubble::onTouch() {
    container->removeChildByTag(getTag());
    int scoreToEarn = scores[age];
    game::sharedGameManager()->correct(scoreToEarn);
}
