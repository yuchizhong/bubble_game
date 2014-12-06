//
//  bubble.cpp
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#include "bubble.h"
#include "game.h"

bubble* bubble::create(string texture, float x, float y, float startingRadius, float rate, float dx, float dy) {
    bubble *b = new (std::nothrow) bubble();
    if (b && b->initWithFile(texture))
    {
        b->autorelease();
        
        b->current_x = x;
        b->current_y = y;
        b->current_r = startingRadius;
        b->r_rate = rate;
        b->delta_x = dx;
        b->delta_y = dy;
        b->timePassed = 0.0f;
        
        b->setPosition(b->current_x, b->current_y);
        b->setScale(b->current_r, b->current_r);
        
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
    this->setPosition(this->current_x, this->current_y);
    this->setScale(this->current_r, this->current_r);
    this->cocos2d::Sprite::setRotation(current_angle);
}

void bubble::onDeath(bool punish) {
    container->removeChildByTag(getTag());
    if (punish)
        game::sharedGameManager()->overOnError();
}

void bubble::onTouch() {
    container->removeChildByTag(getTag());
    game::sharedGameManager()->correct(10);
}
