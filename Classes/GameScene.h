//
//  GameScene.hpp
//  SLG
//
//  Created by Benson on 2017/7/3.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GameScene : public cocos2d::Layer
{
public:
    
    GameScene();
    ~GameScene();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    Sprite *bgSprite;
    Vec2 bgOrigin;
    
    bool comeOut;
    
    void initUI();
    void initListView(ui::ListView *listView);
};

#endif // __HELLOWORLD_SCENE_H__

