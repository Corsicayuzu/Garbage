#ifndef __SETTINGSCENE_SCENE_H__
#define __SETTINGSCENE_SCENE_H__

#include"cocos2d.h"

class SettingLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene*createScene();
	bool init();
	CREATE_FUNC(SettingLayer);
};
#endif
