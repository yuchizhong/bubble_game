//
//  bubble.h
//  game
//
//  Created by Yuchi Zhong on 2014-12-02.
//
//

#ifndef __game__bubble__
#define __game__bubble__

#define BUBBLE_RADIUS 266.0
#define BUBBLE_FILE_RADIUS 400.0
#define EXPLOSION_BEGIN_FRAME 7
#define EXPLOSION_END_FRAME 38
#define EXPLOSION_REMOVE_DELAY 1.0f
#define EXPLOSION_NUM_FRAMES (EXPLOSION_END_FRAME - EXPLOSION_BEGIN_FRAME + 1)
#define EXPLOSION_FRAME_DELAY EXPLOSION_REMOVE_DELAY / (float)EXPLOSION_NUM_FRAMES 

#define MAX_AGE 2

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
    bool punishUser;
    
    static bubble* create(int type, float x, float y, float startingRadius, float rate, float dx, float dy);
    static void startCache();
    void setMyRotation(float init_ro, float ro_rate);
    void update(float dt);
    
    void onDeath(bool punish);
    void onTouch();
    void removeFromLayer(float dt);
};

#endif /* defined(__game__bubble__) */
