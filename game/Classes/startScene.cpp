#include "startScene.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

USING_NS_CC;

#define HALO_POSN_1 visibleSize.height/2 + 100
#define HALO_POSN_2 visibleSize.height/2 + 300

Scene* startScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = startScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool startScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Vec2 touchXY = touch->getLocation();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (touchXY.y > HALO_POSN_1 - 100 && touchXY.y < HALO_POSN_2 + 100) {
        if (touchXY.x < visibleSize.width / 2 - 100) {
            //scroll left
            selectedHalo--;
            if (selectedHalo < 0) {
                selectedHalo = 2;
            }
        } else if (touchXY.x > visibleSize.width / 2 + 100) {
            //scroll right
            selectedHalo++;
            if (selectedHalo > 2) {
                selectedHalo = 0;
            }
        }
        switch (selectedHalo) {
            case 0:
                halo[0]->setPosition(haloPosn[0]);
                halo[1]->setPosition(haloPosn[1]);
                halo[2]->setPosition(haloPosn[2]);
                halo[0]->setScale(1.5, 1.5);
                halo[1]->setScale(1.0, 1.0);
                halo[2]->setScale(1.0, 1.0);
                break;
                
            case 1:
                halo[0]->setPosition(haloPosn[1]);
                halo[1]->setPosition(haloPosn[2]);
                halo[2]->setPosition(haloPosn[0]);
                halo[0]->setScale(1.0, 1.0);
                halo[1]->setScale(1.0, 1.0);
                halo[2]->setScale(1.5, 1.5);
                break;
                
            default:
                halo[0]->setPosition(haloPosn[2]);
                halo[1]->setPosition(haloPosn[0]);
                halo[2]->setPosition(haloPosn[1]);
                halo[0]->setScale(1.0, 1.0);
                halo[1]->setScale(1.5, 1.5);
                halo[2]->setScale(1.0, 1.0);
                break;
        }
    }
    return false;
}

// on "init" you need to initialize your instance
bool startScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    bubble::startCache();
    
    selectedHalo = 0;
    
    auto listener1 = EventListenerTouchOneByOne::create(); //创建一个触摸监听
    listener1->setSwallowTouches(false); //设置是否想下传递触摸
    listener1->onTouchBegan = CC_CALLBACK_2(startScene::onTouchBegan, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("xxh-bg.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale(visibleSize.height/1920.0, visibleSize.height/1920.0);
    this->addChild(sprite, 0);
    
    auto startItem = MenuItemImage::create(
                                           "开始游戏.png",
                                           "开始游戏larged.png",
                                           CC_CALLBACK_1(startScene::startGame, this));
    
    startItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    haloPosn[0] = Vec2(visibleSize.width/2, HALO_POSN_1);
    haloPosn[1] = Vec2(visibleSize.width/4, HALO_POSN_2);
    haloPosn[2] = Vec2(visibleSize.width/4*3, HALO_POSN_2);
    
    halo[0] = Sprite::create("slow.png");
    halo[1] = Sprite::create("fire.png");
    halo[2] = Sprite::create("heath+.png");
    halo[0]->setPosition(haloPosn[0]);
    halo[1]->setPosition(haloPosn[1]);
    halo[2]->setPosition(haloPosn[2]);
    halo[0]->setScale(1.5, 1.5);
    this->addChild(halo[0], 1);
    this->addChild(halo[1], 1);
    this->addChild(halo[2], 1);
    
    return true;
}

void startScene::startGame(cocos2d::Ref *pSender) {
    game::sharedGameManager()->setHalo(selectedHalo);
    game::sharedGameManager()->start();
}
