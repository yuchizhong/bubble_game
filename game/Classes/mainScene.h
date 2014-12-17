#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "bubble.h"
#include "game.h"
#include <list>

using namespace std;

#define TIME_RATE 1.0

class mainScene : public cocos2d::Layer {
    list<bubble*> bubbles;
    
    cocos2d::LabelTTF *lifeLabel;
    cocos2d::LabelTTF *scoreLabel;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void bubbleLogic();
    void generateBubble();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(mainScene);
    
    virtual void update(float tDelta);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
};

#endif
