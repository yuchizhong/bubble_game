//
//  bubble.h
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#ifndef __game__bubble__
#define __game__bubble__

#define BUBBLE_RADIUS 230.0

#include <string>
#include "cocos2d.h"

using namespace std;

class bubble : public cocos2d::Sprite {
public:
    float current_x, current_y;
    float delta_x, delta_y;
    float current_r, r_rate;
    float timePassed;
    cocos2d::Node *container;
    
    static bubble* create(string texture, float x, float y, float startingRadius, float rate, float dx, float dy);
    void update(float dt);
    
    void onDeath(bool punish);
    void onTouch();
};

#endif /* defined(__game__bubble__) */
