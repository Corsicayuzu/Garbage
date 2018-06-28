#ifndef __GAMEPLAYSCENE_SCENE_H__
#define __GAMEPLAYSCENE_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#define LINE 32
#define COLUME 20

class GamePlayLayer : public cocos2d::CCLayerColor
{
public:
	GamePlayLayer();
	~GamePlayLayer();

	static cocos2d::CCScene* createScene();
	virtual bool init();

	void menuCloseCallback(CCObject* pSender);
	CREATE_FUNC(GamePlayLayer);

	//清除整行
	void clearLine(int nLineStart, int nLineEnd);
	//向下拷贝一行
	void copyLine(int nlineNum);
	//检查游戏是否结束
	void checkFail();
	//游戏结束场景
	void gameOver();
	//更新分数显示
	void updateScore();
	//产生一个新的方块
	void newSquareType();
	//变成下一个方块类型
	void nextSquareType();
	//方块下降
	void updateDown(float dt);
	//方块左移
	void updateLeft();
	//方块右移
	void updateRight();

	CCSprite* m_pSquare[LINE][COLUME];	//方块单元格
	int m_nCurline;						//方块所处当前行
	int m_nCurcolume;					//方块所处当前列
	int m_nCurSquareType;				//当前方块类型
	int m_nCurScore;					//当前游戏分数
	CCLabelTTF* m_pLabel;

	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
};

#endif