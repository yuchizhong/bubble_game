#include "mainScene.h"
#include "startScene.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

USING_NS_CC;

#define BUBBLE_GAP 50

static int bubble_tag = 100;
static int genInitialBubbleCount = 0;

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
    
    // return the scene
    return scene;
}

void mainScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *event) {
    for (int i = 0; i < touches.size(); i++) {
        Vec2 touchXY = touches[i]->getLocation();
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        //pause
        if (game::sharedGameManager()->status == INGAME && touchXY.x <= 60 && touchXY.y <= 60) {
            game::sharedGameManager()->pause();
            pauseLabel->setVisible(false);
            pauseMenu->setVisible(true);
            break;
        }
        
        //halo
        if (game::sharedGameManager()->status == INGAME && touchXY.x <= 220 && touchXY.y >= visibleSize.height - 100) {
            game::sharedGameManager()->activateHalo();
        }
        
        //bubble
        if (game::sharedGameManager()->status == INGAME) {
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
        }
    }
}

void mainScene::bubbleLogic() {
    //达到8个泡泡后就不增加了
    if (bubbles.size() >= 8) {
        return;
    }
    
    int gen_bubble_count = 1;
    if (game::sharedGameManager()->halo == 0 && game::sharedGameManager()->halo_active) {
        //no-op
    } else if (game::sharedGameManager()->time_passed < 30.0) {
        int r = getRand(0, 100);
        if (r < 5) {
            gen_bubble_count++;
        }
    } else {
        int r = getRand(0, 100);
        if (r < 10) {
            gen_bubble_count++;
        }
    }
    
    for (int i = 0; i < gen_bubble_count; i++) {
        generateBubble();
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
    
    bubble_tag = 100;
    genInitialBubbleCount = 0;
    
    timeval psv;
    gettimeofday(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
    srand((int)rand_seed);
    
    auto listener1 = EventListenerTouchAllAtOnce::create(); //创建一个触摸监听
    listener1->onTouchesBegan = CC_CALLBACK_2(mainScene::onTouchesBegan, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //add objects
    auto sprite = Sprite::create("xxh-bg.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale(visibleSize.height/1920.0, visibleSize.height/1920.0);
    this->addChild(sprite, 0);
    
    //life
    auto lifeImg = Sprite::create("生命.png");
    lifeImg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height - 60));
    this->addChild(lifeImg, 2);
    
    lifeLabel = LabelTTF::create("体力 5", "Arial", 26);
    lifeLabel->setPosition(Vec2(lifeImg->getContentSize().width / 1.5, lifeImg->getContentSize().height / 2.0 - 1));
    lifeImg->addChild(lifeLabel, 2);
    
    //score
    auto scoreImg = Sprite::create("分数.png");
    scoreImg->setPosition(Vec2(visibleSize.width - 110, visibleSize.height - 60));
    this->addChild(scoreImg, 2);
    
    scoreLabel = LabelTTF::create("0", "Arial", 26);
    scoreLabel->setPosition(Vec2(scoreImg->getContentSize().width / 1.5, scoreImg->getContentSize().height / 2.0 - 1));
    scoreImg->addChild(scoreLabel, 2);
    
    //halo
    haloButtonGray = Sprite::create("道具gray.png");
    haloButtonGray->setPosition(Vec2(110, visibleSize.height - 60));
    //graySprite(haloButtonGray);
    this->addChild(haloButtonGray, 2);
    
    haloButton = ProgressTimer::create(Sprite::create("道具.png"));
    haloButton->setPosition(Vec2(110, visibleSize.height - 60));
    haloButton->setBarChangeRate(Point(1,0));       //设置进程条的变化速率
    haloButton->setType(ProgressTimer::Type::BAR);  //设置进程条的类型
    haloButton->setMidpoint(Point(0,1));            //设置进度的运动方向
    haloButton->setPercentage(0.0f);                //设置初始值为0
    this->addChild(haloButton, 2);
    
    haloLabel = LabelTTF::create(haloName[game::sharedGameManager()->halo], "Arial", 28);
    haloLabel->setPosition(Vec2(haloButton->getContentSize().width / 1.43, haloButton->getContentSize().height / 2.0 - 2));
    haloButton->addChild(haloLabel, 2);
    
    //pause
    pauseLabel = Sprite::create("pause.png");
    pauseLabel->setPosition(Vec2(25, 25));
    this->addChild(pauseLabel, 3);
    
    //pause menu
    auto pauseItem = MenuItemImage::create(
                                           "开始游戏.png",
                                           "开始游戏larged.png",
                                           CC_CALLBACK_1(mainScene::resumeGame, this));
    auto endGameItem = MenuItemImage::create(
                                           "开始游戏.png",
                                           "开始游戏larged.png",
                                           CC_CALLBACK_1(mainScene::endGame, this));
    
    pauseMenu = Menu::create(pauseItem, endGameItem, NULL);
    pauseMenu->alignItemsVertically();
    pauseMenu->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    pauseMenu->setVisible(false);
    this->addChild(pauseMenu, 3);
    
    this->scheduleUpdate();
    
    return true;
}

void mainScene::resumeGame(cocos2d::Ref *pSender) {
    game::sharedGameManager()->resume();
    pauseLabel->setVisible(true);
    pauseMenu->setVisible(false);
}

void mainScene::endGame(cocos2d::Ref *pSender) {
    game::sharedGameManager()->gameOver();
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
    b->setTag(bubble_tag);
    bubble_tag++;
    b->container = this;
    bubbles.push_back(b);
    this->addChild(b, 1);
    lastAddBubbleTime = runningTime;
}

void mainScene::update(float tDelta) {
    if (game::sharedGameManager()->status != INGAME) {
        return;
    }
    
    //6 bubbles on start
    if (genInitialBubbleCount <= 40) {
        if (genInitialBubbleCount % 20 == 0)
            generateBubble();
        genInitialBubbleCount++;
    }
    
    float tIntVal = tDelta * TIME_RATE;
    runningTime += tIntVal;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    for (list<bubble*>::iterator it = bubbles.begin(); it != bubbles.end(); it++) {
        if (game::sharedGameManager()->halo == 0 && game::sharedGameManager()->halo_active)
            (*it)->update(tIntVal / 3.0);
        else
            (*it)->update(tIntVal);
    }
    
    if (game::sharedGameManager()->halo == 0 && game::sharedGameManager()->halo_active && game::sharedGameManager()->halo_time_passed >= 5.0) {
        game::sharedGameManager()->deactivateHalo();
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
    
    game::sharedGameManager()->timePassed(tDelta);
    
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
    
    //halo
    if (game::sharedGameManager()->halo_active) {
        haloButton->setScale(1.15, 1.15);
        haloButtonGray->setScale(1.15, 1.15);
        haloButton->setPercentage(100.0 * (5.0 - game::sharedGameManager()->halo_time_passed) / 5.0);
    } else {
        haloButton->setScale(1.0, 1.0);
        haloButtonGray->setScale(1.0, 1.0);
        haloButton->setPercentage(100.0 * (game::sharedGameManager()->halo_time_passed / 10.0));
    }
}

void mainScene::graySprite(cocos2d::Sprite *sprite) {
    if (sprite) {
        cocos2d::GLProgram *p = new cocos2d::GLProgram();
        p->initWithFilenames("gray.vsh", "gray.fsh");
        p->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
        p->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
        p->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        p->link();
        p->updateUniforms();
        sprite->setGLProgram(p);
    }
}