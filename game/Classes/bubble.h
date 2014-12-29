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
#define EXPLOSION_NUM_FRAMES 31
#define EXPLOSION_FRAME_DELAY (float)EXPLOSION_NUM_FRAMES / 0.5f

#include <string>
#include "cocos2d.h"

using namespace std;

class bubble : public cocos2d::Sprite {
public:
    int bubble_type;
    float current_x, current_y;
    float delta_x, delta_y;
    float current_r, r_rate;
    float current_angle, angle_rate;
    float timePassed;
    int age; //0, 1, 2
    int textureAge;
    cocos2d::Node *container;
    cocos2d::Animate *explosion_animation;
    
    static bubble* create(int type, float x, float y, float startingRadius, float rate, float dx, float dy);
    void setRotation(float init_ro, float ro_rate);
    void update(float dt);
    
    void onDeath(bool punish);
    void removeFromLayer(float dt);
    void onTouch();
};

#endif /* defined(__game__bubble__) */
