#include "startScene.h"
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "mainScene.h"

USING_NS_CC;

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
    
    auto listener1 = EventListenerTouchOneByOne::create(); //创建一个触摸监听
    listener1->setSwallowTouches(false); //设置是否想下传递触摸
    listener1->onTouchBegan = CC_CALLBACK_2(startScene::onTouchBegan, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("xxh-bg.png");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale(1136.0/1920.0, 1136.0/1920.0);
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
    
    return true;
}

void startScene::startGame(cocos2d::Ref *pSender) {
    game::sharedGameManager()->start();
}