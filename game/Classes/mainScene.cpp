#include "mainScene.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

USING_NS_CC;

#define BUBBLE_GAP 30

static int bubble_tag = 100;

static int getRand(int start, int end) {
    float i = CCRANDOM_0_1()*(end-start+1)+start;  //产生一个从start到end间的随机数
    return (int)i;
}

Scene* mainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = mainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
    
    //reset game data
    bubble_tag = 100;
    
    // return the scene
    return scene;
}

bool mainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Vec2 touchXY = touch->getLocation();
    for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
        float dx = abs((*it)->current_x - touchXY.x);
        float dy = abs((*it)->current_y - touchXY.y);
        float distance = sqrt(dx * dx + dy * dy);
        float collide_distance = BUBBLE_RADIUS * (*it)->current_r;
        if (distance <= collide_distance) {
            bubbles.remove(*it);
            (*it)->onTouch();
            //this->removeChildByTag((*it)->getTag());
            bubbleLogic();
            break;
        }
    }
    return false;
}

void mainScene::bubbleLogic() {
    if (game::sharedGameManager()->difficulty_count <= 5) {
        generateBubble();
        int r = getRand(0, 100);
        if (r < 80) {
            generateBubble();
        }
    } else {
        int r = getRand(0, 100);
        generateBubble();
        if (r < 20) {
            generateBubble();
            generateBubble();
        } else if (r < 60) {
            generateBubble();
        }
    }
}

// on "init" you need to initialize your instance
bool mainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    timeval psv;
    gettimeofday(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
    srand((int)rand_seed);
    
    auto listener1 = EventListenerTouchOneByOne::create(); //创建一个触摸监听
    listener1->setSwallowTouches(false); //设置是否想下传递触摸
    listener1->onTouchBegan = CC_CALLBACK_2(mainScene::onTouchBegan, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /*
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    */
    
    auto sprite = Sprite::create("xxh-bg.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale(visibleSize.height/1920.0, visibleSize.height/1920.0);
    this->addChild(sprite, 0);
    
    auto lifeImg = Sprite::create("生命.png");
    lifeImg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height - 60));
    this->addChild(lifeImg, 2);
    
    lifeLabel = LabelTTF::create("体力 5", "Arial", 26);
    lifeLabel->setPosition(Vec2(lifeImg->getContentSize().width / 1.5, lifeImg->getContentSize().height / 2.0 - 1));
    lifeImg->addChild(lifeLabel, 2);
    
    auto scoreImg = Sprite::create("分数.png");
    scoreImg->setPosition(Vec2(visibleSize.width - 110, visibleSize.height - 60));
    this->addChild(scoreImg, 2);
    
    scoreLabel = LabelTTF::create("0", "Arial", 26);
    scoreLabel->setPosition(Vec2(scoreImg->getContentSize().width / 1.5, scoreImg->getContentSize().height / 2.0 - 1));
    scoreImg->addChild(scoreLabel, 2);
    /*
    auto pauseItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(mainScene::menuCloseCallback, this));
    
    pauseItem->setPosition(Vec2(20, 20));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);
    */
    this->scheduleUpdate();
    
    generateBubble();
    
    return true;
}

static double runningTime = 0.0;
static double lastAddBubbleTime = 0.0;

void mainScene::generateBubble() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float resize_factor = visibleSize.width / 640.0 * 500.0 / BUBBLE_RADIUS;
    
    //add bubble
    float boundaryOffsetX = 50;
    float boundaryOffsetY = 80;
    
    float x, y, r;
    int numTry = 0;
    while (numTry < 1000) {
        r = 0.1 * resize_factor + 0.001 * resize_factor * (float)getRand(0, 20);
        float real_r = BUBBLE_RADIUS * r;
        x = real_r + getRand(boundaryOffsetX, visibleSize.width - 2.0 * real_r - boundaryOffsetX);
        y = real_r + getRand(boundaryOffsetY, visibleSize.height - 2.0 * real_r - boundaryOffsetY);
        bool shouldBreak = true;
        if (bubbles.size() == 0) {
            //直接加
            break;
        }
        for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
            float dx = abs((*it)->current_x - x);
            float dy = abs((*it)->current_y - y);
            float distance = sqrt(dx * dx + dy * dy);
            float collide_distance = BUBBLE_RADIUS * (*it)->current_r + BUBBLE_RADIUS * r;
            if (distance < collide_distance + BUBBLE_GAP) {
                shouldBreak = false;
                break;
            }
        }
        if (shouldBreak) {
            break;
        }
        numTry++;
    }
    if (numTry >= 1000) {
        return;
    }
    static int rr = 0;
    rr++;
    if (rr == 3) {
        rr = 0;
    }
    
    bubble *b = bubble::create(rr, x, y,
                               r, 0.1 + 0.001 * (float)getRand(0, 30),
                               10 - (float)getRand(0, 20), 10 - (float)getRand(0, 20));
    b->setRotation((float)getRand(0, 360), 5.0 - (float)getRand(0, 10));
    b->setTag(bubble_tag);
    bubble_tag++;
    b->container = this;
    bubbles.push_back(b);
    this->addChild(b, 1);
    lastAddBubbleTime = runningTime;
}

void mainScene::update(float tDelta) {
    float tIntVal = tDelta * TIME_RATE;
    runningTime += tIntVal;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    if (runningTime - lastAddBubbleTime >= 2.0) {
        //do nothing
    }
    
    for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
        (*it)->update(tIntVal);
    }
    
    //boundary
    for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
        float x = (*it)->current_x;
        float y = (*it)->current_y;
        float r = BUBBLE_RADIUS * (*it)->current_r;
        if (x - r <= 0 || x + r >= visibleSize.width ||
            y - r <= 0 || y + r >= visibleSize.height) {
            bubbles.remove(*it);
            (*it)->onDeath(true);
            //this->removeChildByTag((*it)->getTag());
            break;
        }
    }
    
    //collision
    bool shouldBreak = false;
    for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
        for (list<bubble*>::iterator it2 = bubbles.begin(); it2 != bubbles.end(); it2++) {
            if (*it != *it2) {
                float dx = abs((*it)->current_x - (*it2)->current_x);
                float dy = abs((*it)->current_y - (*it2)->current_y);
                float distance = sqrt(dx * dx + dy * dy);
                float collide_distance = BUBBLE_RADIUS * (*it)->current_r + BUBBLE_RADIUS * (*it2)->current_r;
                if (distance <= collide_distance) {
                    bubbles.remove(*it);
                    //this->removeChildByTag((*it)->getTag());
                    (*it)->onDeath(true);
                    bubbles.remove(*it2);
                    (*it2)->onDeath(false);
                    //this->removeChildByTag((*it2)->getTag());
                    shouldBreak = true;
                    break;
                }
            }
        }
        if (shouldBreak) {
            break;
        }
    }
    
    if (bubbles.size() == 0)
        generateBubble();
    
    int life = MAXERROR - game::sharedGameManager()->error_count;
    char lifeStr[2] = {0};
    sprintf(lifeStr, "%d", life);
    lifeLabel->setString("体力 " + string(lifeStr));
    
    long score = game::sharedGameManager()->score;
    char scoreStr[10] = {0};
    sprintf(scoreStr, "%ld", score);
    scoreLabel->setString(string(scoreStr));
}


void mainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
