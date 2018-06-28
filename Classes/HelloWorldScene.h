#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

typedef enum
{
	MenuItemStart,
	MenuItemSetting
}ActionType;
#include "cocos2d.h"
#include "GameplayScene.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuItemCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
