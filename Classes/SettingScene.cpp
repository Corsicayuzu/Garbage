#include "SettingScene.h"

using namespace cocos2d;

Scene* SettingLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = SettingLayer::create();
	scene->addChild(layer);
	return scene;
}
bool SettingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}
