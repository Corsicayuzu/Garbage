#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SettingScene.h"
#include "GameplayScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size size = Director::getInstance()->getVisibleSize();
	auto* background = Sprite::create("background.jpg");
	background->setPosition(size.width / 2, size.height / 2);
	addChild(background, 0);

	auto hometitle = Sprite::create("title.png");
	hometitle->setPosition(size.width / 2, 500);
	hometitle->setScale(0.6f);
	addChild(hometitle, 1);

	auto producer = Label::createWithTTF("ChenKunyu", "fonts/arial.ttf", 36);
	producer->setPosition(size.width / 2, size.height / 2);
	producer->setScale(0.8f);
	addChild(producer, 1);

	auto startMenuItem = MenuItemImage::create("button1.png", "button1.png", CC_CALLBACK_1(HelloWorld::menuItemCallback, this));
	startMenuItem->setPosition(size.width / 2,200);
	startMenuItem->setTag(ActionType::MenuItemStart);
	startMenuItem->setScale(1.8f);



	auto mu = Menu::create(startMenuItem, NULL);
	mu->setPosition(Point::ZERO);
	addChild(mu, 1);

	return true;
}


void HelloWorld::menuItemCallback(Ref* pSender)
{
	Scene* tsc = nullptr;
	MenuItem*menuItem = (MenuItem*)pSender;
	log("MenuItem Tag=%d", menuItem->getTag());
	switch (menuItem->getTag())
	{
	case ActionType::MenuItemStart:
		//auto scene = GamePlayLayer::createScene();
		tsc = TransitionFade::create(1.0f, GamePlayLayer::createScene());
		log("StartCallback");
		break;

	}
	if (tsc)
		Director::getInstance()->pushScene(tsc);
	//auto scene = SettingLayer::createScene();
	// 让导演 用HelloWorld场景 ☆替换☆ 现在的场景，注意是替换！  
	//Director::getInstance()->replaceScene(scene);
}
