#include "overScene.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "startScene.h"

USING_NS_CC;

#define HALO_POSN_1 visibleSize.height/2 + 100
#define HALO_POSN_2 visibleSize.height/2 + 300

Scene* overScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = overScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool overScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    Vec2 touchXY = touch->getLocation();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    return false;
}

// on "init" you need to initialize your instance
bool overScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener1 = EventListenerTouchOneByOne::create(); //创建一个触摸监听
    listener1->setSwallowTouches(false); //设置是否想下传递触摸
    listener1->onTouchBegan = CC_CALLBACK_2(overScene::onTouchBegan, this);
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
                                           CC_CALLBACK_1(overScene::startGame, this));
    
    startItem->setPosition(Vec2(visibleSize.width/2, visibleSize.height/4));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    char fen[10] = {0};
    sprintf(fen, "%d", game::sharedGameManager()->score);
    auto scoreLabel = LabelTTF::create("得分：" + string(fen), "Arial", 60);
    scoreLabel->setColor(Color3B(231, 179, 37));
    scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.5));
    this->addChild(scoreLabel, 1);
    
    return true;
}

void overScene::startGame(cocos2d::Ref *pSender) {
    auto director = cocos2d::Director::getInstance();
    auto scene = startScene::createScene();
    director->replaceScene(scene);
}