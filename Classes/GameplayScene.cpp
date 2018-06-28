#include "GameplayScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include <vector>

USING_NS_CC;
using namespace std;


GamePlayLayer::GamePlayLayer()
{
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			m_pSquare[i][j] = NULL;
		}
	}

	m_pLabel = NULL;
	m_nCurScore = 0;
	newSquareType();
}

GamePlayLayer::~GamePlayLayer()
{

}

CCScene* GamePlayLayer::createScene()
{
	CCScene *scene = CCScene::create();
	GamePlayLayer *layer = GamePlayLayer::create();
	scene->addChild(layer);


	//extern void Win32SetKeyLayer(GamePlayLayer *layer);
	//Win32SetKeyLayer(layer);

	return scene;
}

bool GamePlayLayer::init()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GamePlayLayer::keyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GamePlayLayer::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto* background = Sprite::create("background1.jpg");
	background->setPosition(547,350);
	addChild(background, 0);

	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
	{
		return false;
	}

	//初始化分数显示
	char buf[4] = { 0 };
	m_nCurScore = 0;
	sprintf(buf, "%d", m_nCurScore);
	m_pLabel = CCLabelTTF::create(buf, "Arial", 42);
	m_pLabel->setColor(ccc3(0, 0, 0));
	m_pLabel->setAnchorPoint(ccp(0, 0));
	m_pLabel->setPosition(ccp(505, 600));
	this->addChild(m_pLabel, 1);

	//初始化格子
	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUME; j++)
		{
			m_pSquare[i][j] = CCSprite::create("square.png");
			m_pSquare[i][j]->setPosition(CCDirector::sharedDirector()->convertToGL(ccp(j * 20 + j * 2 + 10, i * 20 + i * 2 + 10)));
			m_pSquare[i][j]->setTag(0);
			m_pSquare[i][j]->setColor(ccc3(255, 255, 255));
			this->addChild(m_pSquare[i][j]);
		}
	}

	this->schedule(schedule_selector(GamePlayLayer::updateDown), 1.0);

	return true;
}

void GamePlayLayer::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

void GamePlayLayer::updateScore()
{
	char buf[8] = { 0 };
	sprintf(buf, "%d", m_nCurScore);
	m_pLabel->setString(buf);
}

void GamePlayLayer::clearLine(int nLineStart, int nLineEnd)
{
	//检查游戏是否结束
	checkFail();

	for (int i = nLineStart; i <= nLineEnd; i++)
	{
		int j;
		for (j = 0; j < COLUME; j++)
		{
			if (i > -1 && m_pSquare[i][j]->getTag() == 0)
			{
				break;
			}
		}

		if (COLUME == j)
		{
			//清除一行
			for (int k = i - 1; k >= 0; k--)
			{
				copyLine(k);
			}
			for (int x = 0; x < COLUME; x++)
			{
				m_pSquare[0][x]->setColor(ccc3(255, 255, 255));
				m_pSquare[0][x]->setTag(0);
			}
			m_nCurScore++;
		}
	}

	updateScore();
}

void GamePlayLayer::copyLine(int nlineNum)
{
	for (int i = 0; i < COLUME; i++)
	{
		m_pSquare[nlineNum + 1][i]->setColor(m_pSquare[nlineNum][i]->getColor());
		m_pSquare[nlineNum + 1][i]->setTag(m_pSquare[nlineNum][i]->getTag());
	}
}

void GamePlayLayer::newSquareType()
{
	m_nCurSquareType = rand() % 19 + 1;
	switch (m_nCurSquareType)
	{
	case 1:
	case 3:
	case 4:
	case 9:
	case 10:
	case 11:
	case 12:
	case 15:
	case 16:
		m_nCurline = 0;
		m_nCurcolume = 3;
		break;
	case 2:
	case 5:
	case 6:
	case 7:
	case 8:
	case 13:
	case 14:
	case 17:
	case 18:
	case 19:
		m_nCurline = 0;
		m_nCurcolume = 4;
		break;
	}
}

void GamePlayLayer::nextSquareType()
{
	switch (m_nCurSquareType)
	{
	case 1:
		//判断是否可以变换
		if (m_nCurline + 1 > LINE - 1)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && m_nCurline - 2 + i > -1 && m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
			if (i != 1 && m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(52, 228, 249));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(1);
			}
		}

		m_nCurline += 2;
		m_nCurcolume++;
		m_nCurSquareType = 2;
		break;
	case 2:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0 || m_nCurcolume + 2 > COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (i != 1 && m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume - 1 + i]->getTag() == 1)
			{
				return;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != 2 && m_nCurline - 4 + i > -1)
			{
				m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setTag(0);
			}
			if (i != 1 && m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume - 1 + i]->setColor(ccc3(52, 228, 249));
				m_pSquare[m_nCurline - 2][m_nCurcolume - 1 + i]->setTag(1);
			}
		}

		m_nCurline--;
		m_nCurcolume--;
		m_nCurSquareType = 1;
		break;
	case 3:
		//判断是否可以变换
		if (m_nCurline > LINE - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->getTag() == 1)
			{
				return;
			}
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
			}
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setTag(1);
			}
		}

		m_nCurline++;
		m_nCurcolume++;
		m_nCurSquareType = 4;
		break;
	case 4:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1 && m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 > -1)
			{
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setTag(0);
			}
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(1);
			}
		}

		m_nCurcolume--;
		m_nCurSquareType = 5;
		break;
	case 5:
		//判断是否可以变换
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 3 > -1 && m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(0);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(1);
			}
		}

		m_nCurSquareType = 6;
		break;
	case 6:
		//判断是否可以变换
		if (m_nCurcolume + 2 > COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i > -1 && m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i > -1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(1);
			}
		}

		m_nCurline--;
		m_nCurSquareType = 3;
		break;
	case 7:
		//判断是否可以变换
		if (m_nCurline > LINE - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(0);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->setTag(1);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurline++;
		m_nCurcolume++;
		m_nCurSquareType = 8;
		break;
	case 8:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1 && m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setTag(1);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurcolume--;
		m_nCurSquareType = 9;
		break;
	case 9:
		//判断是否可以变换
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 > -1 && m_pSquare[m_nCurline - 3][m_nCurcolume + i]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 > -1)
			{
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setTag(1);
			}
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurSquareType = 10;
		break;
	case 10:
		//判断是否可以变换
		if (m_nCurcolume + 2 > COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i > -1 && m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 > -1)
			{
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setTag(0);
			}
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i > -1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setTag(1);
			}
		}

		m_nCurline--;
		m_nCurSquareType = 7;
		break;
	case 11:
		//判断是否可以变换
		if (m_nCurline > LINE - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 + i > -1 && m_pSquare[m_nCurline - 1 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 + i > -1)
			{
				m_pSquare[m_nCurline - 1 + i][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
				m_pSquare[m_nCurline - 1 + i][m_nCurcolume + 2]->setTag(1);
			}
		}

		m_nCurline++;
		m_nCurcolume++;
		m_nCurSquareType = 12;
		break;
	case 12:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume - 1 + i]->getTag() == 1)
			{
				return;
			}
		}

		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume - 1 + i]->setColor(ccc3(26, 242, 26));
				m_pSquare[m_nCurline - 1][m_nCurcolume - 1 + i]->setTag(1);
			}
		}

		m_nCurcolume--;
		m_nCurSquareType = 11;
		break;
	case 13:
		//判断是否可以变换
		if (m_nCurline > LINE - 1)
		{
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setTag(0);
			}
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);
		}

		m_nCurline++;
		m_nCurcolume++;
		m_nCurSquareType = 14;
		break;
	case 14:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0)
		{
			return;
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i > -1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurcolume--;
		m_nCurSquareType = 13;
		break;
	case 15:
		//判断是否可以变换
		if (m_nCurline > LINE - 1)
		{
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);

		m_nCurline++;
		m_nCurcolume++;
		m_nCurSquareType = 16;
		break;
	case 16:
		//判断是否可以变换
		if (m_nCurcolume - 1 < 0)
		{
			return;
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}

		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		}

		m_nCurcolume--;
		m_nCurSquareType = 17;
		break;
	case 17:
		//判断是否可以变换
		if (m_nCurline - 3 > -1 && m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurSquareType = 18;
		break;
	case 18:
		//判断是否可以变换
		if (m_nCurcolume + 2 > COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);
		}

		m_nCurline--;
		m_nCurSquareType = 15;
		break;
	case 19:
		break;
	}
}

void GamePlayLayer::gameOver()
{
	GameOverScene *gameOverScene = GameOverScene::create();
	gameOverScene->getLayer()->getLabel()->setString("You Lose!");
	CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void GamePlayLayer::checkFail()
{
	switch (m_nCurSquareType)
	{
	case 1:
		if (m_nCurline - 1 < 0)
		{
			gameOver();
		}
		break;
	case 2:
		if (m_nCurline - 4 < 0)
		{
			gameOver();
		}
		break;
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
	case 19:
		if (m_nCurline - 2 < 0)
		{
			gameOver();
		}
		break;
	case 4:
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
		if (m_nCurline - 3 < 0)
		{
			gameOver();
		}
		break;
	}
}

void GamePlayLayer::updateDown(float dt)
{
	switch (m_nCurSquareType)
	{
	case 1:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 1, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 1, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 4; i++)
		{
			//消除原色
			if (m_nCurline < LINE && m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
			//显示新色
			if (m_nCurline < LINE)
			{
				m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(52, 228, 249));
				m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
			}
		}

		m_nCurline++;
		break;
	case 2:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 4, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 4, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		//消除原色
		if (m_nCurline < LINE && m_nCurline - 4 > -1)
		{
			m_pSquare[m_nCurline - 4][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 4][m_nCurcolume]->setTag(0);
		}
		//显示新色
		if (m_nCurline < LINE)
		{
			m_pSquare[m_nCurline][m_nCurcolume]->setColor(ccc3(52, 228, 249));
			m_pSquare[m_nCurline][m_nCurcolume]->setTag(1);
		}

		m_nCurline++;
		break;
	case 3:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			//消除原色
			if (i != 0 && m_nCurline < LINE && m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
			//显示新色
			if (m_nCurline < LINE)
			{
				m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		}

		m_nCurline++;
		break;
	case 4:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1 ||
			(m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->getTag() == 1))
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (m_nCurline - 3 > -1)
			{
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setTag(0);
			}
		}
		//显示新色
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(1);
		}
		m_pSquare[m_nCurline][m_nCurcolume]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline][m_nCurcolume]->setTag(1);

		m_nCurline++;
		break;
	case 5:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 2]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setTag(0);
			}
			//显示新色
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(1);
			}
		}
		m_pSquare[m_nCurline][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline][m_nCurcolume + 2]->setTag(1);
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		}

		m_nCurline++;
		break;
	case 6:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 3, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);
		}

		m_nCurline++;
		break;
	case 7:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
		}

		m_nCurline++;
		break;
	case 8:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 3, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
		}
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		}

		m_nCurline++;
		break;
	case 9:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 1 + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setTag(0);
			}
			//显示新色
			if (m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + 1 + i]->setColor(ccc3(245, 30, 217));
				m_pSquare[m_nCurline - 1][m_nCurcolume + 1 + i]->setTag(1);
			}
		}
		m_pSquare[m_nCurline][m_nCurcolume]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline][m_nCurcolume]->setTag(1);
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		}

		m_nCurline++;
		break;
	case 10:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1 ||
			(m_nCurline - 2 > -1 && m_pSquare[m_nCurline - 2][m_nCurcolume]->getTag() == 1))
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			//消除原色
			if (m_nCurline - 3 > -1)
			{
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3][m_nCurcolume + i]->setTag(0);
			}
		}
		//显示新色
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(1);
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);

		m_nCurline++;
		break;
	case 11:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + 1 + i]->setTag(0);
			}
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(1);
		}

		m_nCurline++;
		break;
	case 12:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);

		m_nCurline++;
		break;
	case 13:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setTag(0);
			}
			m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline][m_nCurcolume + 1 + i]->setTag(1);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		}

		m_nCurline++;
		break;
	case 14:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline][m_nCurcolume]->setTag(1);

		m_nCurline++;
		break;
	case 15:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume + i]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
			if (m_nCurline - 1 > -1 && i != 1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(0);
			}
		}
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);
		}

		m_nCurline++;
		break;
	case 16:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline][m_nCurcolume]->setTag(1);
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);
		}

		m_nCurline++;
		break;
	case 17:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->getTag() == 1)
		{
			clearLine(m_nCurline - 2, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setTag(0);
			}
			if (i != 1 && m_nCurline - 1 > -1)
			{
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setColor(ccc3(233, 178, 11));
				m_pSquare[m_nCurline - 1][m_nCurcolume + i]->setTag(1);
			}
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);

		m_nCurline++;
		break;
	case 18:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 3, LINE - 1);
			newSquareType();
			return;
		}
		if (m_pSquare[m_nCurline][m_nCurcolume + 1]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}
		if (m_nCurline - 1 > -1 && m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			clearLine(m_nCurline - 3, m_nCurline - 1);
			newSquareType();
			return;
		}

		//下降一格
		if (m_nCurline - 3 > -1)
		{
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline][m_nCurcolume + 1]->setTag(1);
		if (m_nCurline - 2 > -1)
		{
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		}
		if (m_nCurline - 1 > -1)
		{
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		}

		m_nCurline++;
		break;
	case 19:
		//判断是否允许继续下降
		if (m_nCurline >= LINE)
		{
			clearLine(LINE - 2, LINE - 1);
			newSquareType();
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_pSquare[m_nCurline][m_nCurcolume]->getTag() == 1)
			{
				clearLine(m_nCurline - 2, m_nCurline - 1);
				newSquareType();
				return;
			}
		}

		//下降一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline][m_nCurcolume + i]->setColor(ccc3(244, 69, 46));
			m_pSquare[m_nCurline][m_nCurcolume + i]->setTag(1);
			if (m_nCurline - 2 > -1)
			{
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2][m_nCurcolume + i]->setTag(0);
			}
		}

		m_nCurline++;
		break;
	}
}

void GamePlayLayer::updateLeft()
{
	switch (m_nCurSquareType)
	{
	case 1:
		//判断是否允许继续左移
		if (m_nCurline - 1 < 0 || m_nCurcolume <= 0 || m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}

		//左移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setColor(ccc3(52, 228, 249));
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setTag(0);

		m_nCurcolume--;
		break;
	case 2:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_nCurline - 4 + i < 0 || m_pSquare[m_nCurline - 4 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 4; i++)
		{
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume - 1]->setColor(ccc3(52, 228, 249));
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume - 1]->setTag(1);
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setTag(0);
		}

		m_nCurcolume--;
		break;
	case 3:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setTag(1);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);

		m_nCurcolume--;
		break;
	case 4:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setTag(1);

			if (i != 2 && m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 5:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i > -1)
			{
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);

		m_nCurcolume--;
		break;
	case 6:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(1);
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 7:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(0);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setTag(1);

		m_nCurcolume--;
		break;
	case 8:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setTag(1);
			if (i != 2)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 9:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setTag(1);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume--;
		break;
	case 10:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(1);
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume - 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 3][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 11:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);

		m_nCurcolume--;
		break;
	case 12:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);

		m_nCurcolume--;
		break;
	case 13:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);

		m_nCurcolume--;
		break;
	case 14:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume--;
		break;
	case 15:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 1][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 16:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume - 1]->setTag(1);
			if (i != 1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume--;
		break;
	case 17:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);

		m_nCurcolume--;
		break;
	case 18:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume]->getTag() == 1)
		{
			return;
		}

		//左移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
			if (i != 1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(233, 178, 11));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(1);
			}
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume - 1]->setTag(1);

		m_nCurcolume--;
		break;
	case 19:
		//判断是否允许继续左移
		if (m_nCurcolume <= 0)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->getTag() == 1)
			{
				return;
			}
		}

		//左移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setColor(ccc3(244, 69, 46));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume - 1]->setTag(1);
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(0);
		}

		m_nCurcolume--;
		break;
	}
}

void GamePlayLayer::updateRight()
{
	switch (m_nCurSquareType)
	{
	case 1:
		//判断是否允许继续右移
		if (m_nCurline - 1 < 0 || m_nCurcolume + 3 >= COLUME - 1 || m_pSquare[m_nCurline - 1][m_nCurcolume + 3 + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3 + 1]->setColor(ccc3(52, 228, 249));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3 + 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 2:
		//判断是否允许继续右移
		if (m_nCurcolume >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (m_nCurline - 4 + i < 0 || m_pSquare[m_nCurline - 4 + i][m_nCurcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 4; i++)
		{
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume + 1]->setColor(ccc3(52, 228, 249));
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume + 1]->setTag(1);
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 4 + i][m_nCurcolume]->setTag(0);
		}

		m_nCurcolume++;
		break;
	case 3:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setTag(1);

		m_nCurcolume++;
		break;
	case 4:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 1]->setTag(1);
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 3][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 3][m_nCurcolume + 2]->setTag(1);

		m_nCurcolume++;
		break;
	case 5:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->setTag(1);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(0);

		m_nCurcolume++;
		break;
	case 6:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setTag(1);
			if (i != 2)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 7:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 3]->setTag(1);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 8:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(1);
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(1);

		m_nCurcolume++;
		break;
	case 9:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(245, 30, 217));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);

		m_nCurcolume++;
		break;
	case 10:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setColor(ccc3(245, 30, 217));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setTag(1);
			if (i != 0)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 11:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		//右移一格
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 12:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume++;
		break;
	case 13:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		//右移一格
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume++;
		break;
	case 14:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setColor(ccc3(26, 242, 26));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setTag(1);
		}
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
		}
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(26, 242, 26));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(1);
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume++;
		break;
	case 15:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		//右移一格
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 3]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume++;
		break;
	case 16:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 3 < 0 || m_pSquare[m_nCurline - 3][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->getTag() == 1)
		{
			return;
		}

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume]->setTag(0);
			if (i != 1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(233, 178, 11));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(1);
			}
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 2]->setTag(1);

		m_nCurcolume++;
		break;
	case 17:
		//判断是否允许继续右移
		if (m_nCurcolume + 2 >= COLUME - 1)
		{
			return;
		}
		if (m_nCurline - 2 < 0 || m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->getTag() == 1)
		{
			return;
		}
		if (m_nCurline - 1 < 0 || m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->getTag() == 1)
		{
			return;
		}

		//右移一格
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 2][m_nCurcolume + 3]->setTag(1);
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setColor(ccc3(233, 178, 11));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 2]->setTag(1);
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 1][m_nCurcolume + 1]->setTag(0);

		m_nCurcolume++;
		break;
	case 18:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 3; i++)
		{
			if (m_nCurline - 3 + i < 0 || m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 3; i++)
		{
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setColor(ccc3(233, 178, 11));
			m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 2]->setTag(1);
			if (i != 1)
			{
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setColor(ccc3(255, 255, 255));
				m_pSquare[m_nCurline - 3 + i][m_nCurcolume + 1]->setTag(0);
			}
		}
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setColor(ccc3(255, 255, 255));
		m_pSquare[m_nCurline - 2][m_nCurcolume]->setTag(0);

		m_nCurcolume++;
		break;
	case 19:
		//判断是否允许继续右移
		if (m_nCurcolume + 1 >= COLUME - 1)
		{
			return;
		}
		for (int i = 0; i < 2; i++)
		{
			if (m_nCurline - 2 + i < 0 || m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->getTag() == 1)
			{
				return;
			}
		}

		//右移一格
		for (int i = 0; i < 2; i++)
		{
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setColor(ccc3(244, 69, 46));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume + 2]->setTag(1);
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setColor(ccc3(255, 255, 255));
			m_pSquare[m_nCurline - 2 + i][m_nCurcolume]->setTag(0);
		}

		m_nCurcolume++;
		break;
	}
}

void GamePlayLayer::keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		updateLeft();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		updateDown(0.0);
	else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
		nextSquareType();
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		updateRight();
}
void GamePlayLayer::keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_W)  //按键弹出
	{

	}
}