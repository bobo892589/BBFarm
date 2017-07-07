//
//  GameScene.cpp
//  SLG
//
//  Created by Benson on 2017/7/3.
//
//

#include "GameScene.h"
//#include "cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include <iostream>

USING_NS_CC;

using namespace ui;



Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
GameScene::GameScene()
//:buyTarget(NULL)
:comeOut(false)
//,createTarget(false)
//,deltax(0)
//,deltay(0)
//,longPress(false)
{
}

GameScene::~GameScene()
{
}
// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto mapLayer = LayerColor::create(Color4B(78, 127, 41, 255));
    this->addChild(mapLayer);
    
    bgSprite = Sprite::create("2.jpg");
    bgSprite->setAnchorPoint(Vec2::ZERO);
    bgSprite->setPosition(Vec2::ZERO);
    bgOrigin = Vec2(Vec2::ZERO);
    mapLayer->addChild(bgSprite);
    
    auto map = TMXTiledMap::create("map1.tmx");
    mapLayer->setContentSize(map->getContentSize());
    bgSprite->addChild(map, 10);
    
    auto treeSprite = Sprite::create("1.png");
    treeSprite->setAnchorPoint(Vec2(0, 0));
    treeSprite->setPosition(Vec2(0, 0));
    treeSprite->setScale(2);
    
    
    bgSprite->addChild(treeSprite);
    
    //绑定事件
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bgSprite);
    
    
    return true;
}

void GameScene::initUI() {
//    comeOut
    auto listView = ListView::create();
    initListView(listView);
    addChild(listView, 100);
    
    
}

void GameScene::initListView(ui::ListView *listView) {
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(true);
    listView->setContentSize(Size(300, 600));
    for (int i = 10; i < 0; i ++) {
        auto itemLayout = Layout::create();
        listView->addChild(itemLayout);
    }
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
}


void GameScene::onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event) {
    auto winSize = Director::getInstance()->getWinSize();
    if (touches.size() > 1) {
        // 得到当前两触摸点
        auto point1 = touches[0]->getLocation();
        auto point2 = touches[1]->getLocation();
        // 计算两点之间得距离
        auto currDistance = point1.distance(point2);
        // 计算两触摸点上一时刻之间得距离
        auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
        // 两触摸点与原点的差向量，pointVec1和pointVec2是相对于bgSprite的位置
        auto pointVec1 = point1  - bgOrigin;
        auto pointVec2 = point2  - bgOrigin;
        // 两触摸点的相对中点
        auto relMidx = (pointVec1.x + pointVec2.x) / 2 ;
        auto relMidy = (pointVec1.y + pointVec2.y) / 2 ;
        // 计算bgSprite的锚点
        auto anchorX = relMidx / bgSprite->getBoundingBox().size.width;
        auto anchorY = relMidy / bgSprite->getBoundingBox().size.height;
        // 相对屏幕的中点
        auto absMidx = (point2.x + point1.x) / 2 ;
        auto absMidy = (point2.y + point1.y) / 2 ;
        
        // 缩放时，为了避免出现空白的区域，需要做以下的边界处理。
        // 当bgSprite快要进入到屏幕时，修改bgSprite的位置（既absMidx和absMidy）。
        if(  bgOrigin.x > 0)
        {
            absMidx -= bgOrigin.x;
        }
        if( bgOrigin.x < -bgSprite->getBoundingBox().size.width + winSize.width )
        {
            absMidx +=  -bgSprite->getBoundingBox().size.width + winSize.width - bgOrigin.x;
        }
        if( bgOrigin.y > 0 )
        {
            absMidy -= bgOrigin.y;
        }
        if( bgOrigin.y < -bgSprite->getBoundingBox().size.height + winSize.height )
        {
            absMidy +=  -bgSprite->getBoundingBox().size.height + winSize.height - bgOrigin.y;
        }
        // 重设bgSprite锚点和位置
        bgSprite->setAnchorPoint(Vec2(anchorX, anchorY));
        bgSprite->setPosition(Vec2(absMidx, absMidy));
        // 根据两触摸点前后的距离计算缩放倍率
        auto scale = bgSprite->getScale() * ( currDistance / prevDistance);
        // 控制缩放倍率在1～4倍之间，最小倍率不能太小，不让背景将不能填充满整个屏幕。
        scale = MIN(4,MAX(1, scale));
        bgSprite->setScale(scale);
        // 更新原点位置
        bgOrigin = Vec2(absMidx, absMidy) - Vec2(bgSprite->getBoundingBox().size.width * anchorX, bgSprite->getBoundingBox().size.height * anchorY) ;
    } else if (touches.size() == 1)
    {
        // 单点时，touches中只有一个Touch对象，所以通过touches[0]就可以得到触摸对象
        auto touch = touches[0];
        // 计算滑动过程中的滑动增量
        auto diff = touch->getDelta();
        // 得到当前bgSprite的位置
        auto currentPos = bgSprite->getPosition();
        // 得到滑动后bgSprite应该所在的位置
        auto pos = currentPos + diff;
        // 得到此刻bgSprite的尺寸
        auto bgSpriteCurrSize = bgSprite->getBoundingBox().size;
        
        //边界控制，约束pos的位置
        pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
        pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
        pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
        pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
        // 重设bgSprite位置
        bgSprite->setPosition(pos);
        
        // 更新原点位置
        if( pos.x >= bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x
           || pos.x <= -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x)
        {
            diff.x = 0;
        }
        if( pos.y >= bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y
           || pos.y <= -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y)
        {
            diff.y = 0;
        }
        bgOrigin += diff;
    }
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
